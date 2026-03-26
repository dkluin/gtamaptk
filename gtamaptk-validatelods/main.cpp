#include <gtamaptk-core.h>
#include <stdexcept>

#define TRY_LOG_IF_FAILED(expr) if (!expr) { printf("%s\n", GetLastErrorMsg()); return 1; }

float GetObjectDrawDist(IdeFile mIdeFile, int32_t nModelIndex, bool& bFound) {
	for (auto& it : mIdeFile.m_vObjsEntries) {
		if (it.m_nModelIndex == nModelIndex) {
			bFound = true;
			return it.m_fDrawDistance[0];
		}
	}

	for (auto& it : mIdeFile.m_vTobjEntries) {
		if (it.m_nModelIndex == nModelIndex) {
			bFound = true;
			return it.m_fDrawDistance[0];
		}
	}

	for (auto& it : mIdeFile.m_vAnimEntries) {
		if (it.m_nModelIndex == nModelIndex) {
			bFound = true;
			return it.m_fDrawDistance;
		}
	}

	bFound = false;
	return -999.0f;
}

int main(int argc, char* argv[]) {
	IdeFile mIdeFile;
	IplFile mTextIpl, mBinaryIpl;
	float fDrawDist;
	bool bFoundInIde;

	enum {
		ARG_APP_NAME = 0,
		ARG_INPUT_IDE,
		ARG_INPUT_TEXT_IPL,
		ARG_INPUT_BINARY_IPL_MODE,
		ARG_INPUT_BINARY_IPL
	};

	if (argc < 4) {
		printf("Usage: %s [input IDE] [input text IPL] [input binary IPL mode, -b for binary and -t for text] [input binary IPL]\n", argv[ARG_APP_NAME]);
		return 1;
	}

	TRY_LOG_IF_FAILED(mIdeFile.Read(argv[ARG_INPUT_IDE]));
	TRY_LOG_IF_FAILED(mTextIpl.Read(argv[ARG_INPUT_TEXT_IPL]));

	if (!strcmp(argv[ARG_INPUT_BINARY_IPL_MODE], "-b")) {
		TRY_LOG_IF_FAILED(mBinaryIpl.ReadBinary(argv[ARG_INPUT_BINARY_IPL]));
	} else if (!strcmp(argv[ARG_INPUT_BINARY_IPL_MODE], "-t")) {
		TRY_LOG_IF_FAILED(mBinaryIpl.Read(argv[ARG_INPUT_BINARY_IPL]));
	} else {
		printf("Unknown binary IPL input mode: %s\n", argv[ARG_INPUT_BINARY_IPL_MODE]);
		return 1;
	}

	for (int32_t i = 0; i < mBinaryIpl.m_vInstEntries.size(); i++) {
		IplInstEntry mCurrInst = mBinaryIpl.m_vInstEntries.at(i);

		if (mCurrInst.m_nLodIndex == -1) continue;

		if (mCurrInst.m_nLodIndex < 0) {
			printf("Binary IPL entry %d: Model %d at %f %f %f has negative LOD index (%d) - this will cause an out-of-bounds read on the entity index array and thus UB\n", 
				i, mCurrInst.m_nModelIndex, mCurrInst.m_vPosition.x, mCurrInst.m_vPosition.y, mCurrInst.m_vPosition.z, mCurrInst.m_nLodIndex);
			continue;
		}

		IplInstEntry mLod;
		try {
			mLod = mTextIpl.m_vInstEntries.at(mCurrInst.m_nLodIndex);
		} catch (std::out_of_range e) {
			printf("Binary IPL entry %d: Model %d at %f %f %f has an out of range LOD index (%d) - this will cause an out-of-bounds read on the entity index array and thus UB\n",
				i, mCurrInst.m_nModelIndex, mCurrInst.m_vPosition.x, mCurrInst.m_vPosition.y, mCurrInst.m_vPosition.z, mCurrInst.m_nLodIndex);
			continue;
		}

		bFoundInIde = false;
		fDrawDist = GetObjectDrawDist(mIdeFile, mCurrInst.m_nModelIndex, bFoundInIde);

		if (!bFoundInIde) {
			printf("Binary IPL entry %d: Model %d not found in IDE\n", i, mCurrInst.m_nModelIndex);
		} else if (fDrawDist >= 300.0f) {
			printf("Binary IPL entry %d: Model %d has a draw distance above 300 units (%f)\n", i, mCurrInst.m_nModelIndex, fDrawDist);
		}
		
		bFoundInIde = false;
		fDrawDist = GetObjectDrawDist(mIdeFile, mLod.m_nModelIndex, bFoundInIde);

		if (!bFoundInIde) {
			printf("Text IPL entry %d: Model %d not found in IDE\n", mCurrInst.m_nLodIndex, mLod.m_nModelIndex);
		} else if (fDrawDist <= 300.0f) {
			printf("Text IPL entry %d: Model %d has a draw distance below 300 units (%f)\n", mCurrInst.m_nLodIndex, mLod.m_nModelIndex, fDrawDist);
		}

		if (mLod.m_nLodIndex != -1) {
			printf("Text IPL entry %d: Model %d at %f %f %f is has been assigned as an LOD, but has an LOD entry of its own. This causes the entity to be deleted twice due to how the game cleans up the entity lists in CWorld::ShutDown, resulting in a shutdown crash.\n",
				mCurrInst.m_nLodIndex, mLod.m_nModelIndex, mLod.m_vPosition.x, mLod.m_vPosition.y, mLod.m_vPosition.z);
			continue;
		}
	}

	return 0;
}