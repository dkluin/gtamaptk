#include <gtamaptk-core.h>

#define TRY_LOG_IF_FAILED(expr) if (!expr) { printf("%s\n", GetLastErrorMsg()); return 1; }

bool ValidateFloat(const char* szStr) {
	int32_t nNumCharactersRead;
	float fTemp;

	return sscanf(szStr, "%f %n", &fTemp, &nNumCharactersRead) == 1 && nNumCharactersRead == strlen(szStr);
}

bool PointOverlapsWithinThreshold(const Vector3d& a, const Vector3d& b, float fThreshold) {
	bool bOverlaps = true;

	if (fabs(a.x - b.x) > fThreshold) bOverlaps = false;
	if (fabs(a.y - b.y) > fThreshold) bOverlaps = false;
	if (fabs(a.z - b.z) > fThreshold) bOverlaps = false;
	return bOverlaps;
}

bool ShouldLinkLOD(IplInstEntry mTextIplEntry, IplInstEntry mBinaryIplEntry, float fThreshold) {
	char szTmp[MAX_IMG_ENTRY_NAME];

	if (strlen(mBinaryIplEntry.m_szModelName) < 4) return false;
	strcpy(szTmp, mBinaryIplEntry.m_szModelName);
	strncpy(szTmp, "LOD", 3);

	return !strcmp(szTmp, mTextIplEntry.m_szModelName) 
		&& mBinaryIplEntry.m_nAreaID == mTextIplEntry.m_nAreaID 
		&& PointOverlapsWithinThreshold(mBinaryIplEntry.m_vPosition, mTextIplEntry.m_vPosition, fThreshold);
}

int main(int argc, char* argv[]) {
	std::vector<IplFile> vBinaryIpls;
	float fMaxAllowedPosDifference;
	IplFile mTextIpl;

	enum {
		ARG_APP_NAME = 0,
		ARG_TEXT_IPL,
		ARG_POS_MAX_ALLOWED_DIFFERENCE, // The maximum allowed difference in position for a model to still be linked to an LOD
		ARG_BINARY_IPL_START
	};

	if (argc < 3) {
		printf("Usage: %s [input.ipl] [max allowed position difference as a float] [decompiled input_stream0.ipl] [decompiled input_stream1.ipl] ...", argv[ARG_APP_NAME]);
		return 1;
	}

	if (!ValidateFloat(argv[ARG_POS_MAX_ALLOWED_DIFFERENCE])) {
		printf("Error: Value \"%s\" is not a valid float\n", argv[ARG_POS_MAX_ALLOWED_DIFFERENCE]);
	}

	fMaxAllowedPosDifference = strtof(argv[ARG_POS_MAX_ALLOWED_DIFFERENCE], nullptr);
	TRY_LOG_IF_FAILED(mTextIpl.Read(argv[ARG_TEXT_IPL]));

	for (int32_t i = ARG_BINARY_IPL_START; i < argc; i++) {
		IplFile mBinaryIpl;
		TRY_LOG_IF_FAILED(mBinaryIpl.Read(argv[i]));

		vBinaryIpls.push_back(mBinaryIpl);
	}

	for (auto& it : vBinaryIpls) {
		for (auto& jt : it.m_vInstEntries) {
			int32_t nFoundLodIndex = -1;
			for (int32_t i = 0; i < mTextIpl.m_vInstEntries.size(); i++) {
				IplInstEntry mTextIplEntry = mTextIpl.m_vInstEntries.at(i);

				if (ShouldLinkLOD(mTextIplEntry, jt, fMaxAllowedPosDifference)) {
					nFoundLodIndex = i;
					break;
				}
			}

			jt.m_nLodIndex = nFoundLodIndex;
		}
	}

	for (int32_t i = 0; i < vBinaryIpls.size(); i++) {
		TRY_LOG_IF_FAILED(vBinaryIpls[i].Write(argv[ARG_BINARY_IPL_START + i]));
	}

	return 0;
}