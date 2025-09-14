#include <gtamaptk-core.h>

#define TRY_LOG_IF_FAILED(expr) if (!expr) { printf("%s\n", GetLastErrorMsg()); return 1; }

bool ValidateNumber(const char* szValue) {
	if (!*szValue || *szValue == '-' || *szValue == '\0') return false; // empty strings / negative numbers are invalid

	// check if the string contains only digits
	while (*szValue) {
		if (!isdigit(*szValue)) return false;
		else ++szValue;
	}

	return true;
}

void MoveAudioZone(IplAuzoEntry& mEntry, float fOffsetX, float fOffsetY, float fOffsetZ) {
	switch (mEntry.m_eType)	{
		case IPLAUZO_BOX:
			printf("AUZO: Changing audio box from (%.2f %.2f %.2f - %.2f %.2f %.2f) to (%.2f %.2f %.2f - %.2f %.2f %.2f)\n",
				mEntry.m_mBox.m_vMin.x, mEntry.m_mBox.m_vMin.y, mEntry.m_mBox.m_vMin.z,
				mEntry.m_mBox.m_vMax.x, mEntry.m_mBox.m_vMax.y, mEntry.m_mBox.m_vMax.z,
				mEntry.m_mBox.m_vMin.x + fOffsetX, mEntry.m_mBox.m_vMin.y + fOffsetY, mEntry.m_mBox.m_vMin.z + fOffsetZ,
				mEntry.m_mBox.m_vMax.x + fOffsetX, mEntry.m_mBox.m_vMax.y + fOffsetY, mEntry.m_mBox.m_vMax.z + fOffsetZ);

			mEntry.m_mBox.m_vMin.x += fOffsetX;
			mEntry.m_mBox.m_vMin.y += fOffsetY;
			mEntry.m_mBox.m_vMin.z += fOffsetZ;
			mEntry.m_mBox.m_vMax.x += fOffsetX;
			mEntry.m_mBox.m_vMax.y += fOffsetY;
			mEntry.m_mBox.m_vMax.z += fOffsetZ;
			break;

		case IPLAUZO_SPHERE:
			printf("AUZO: Changing audio sphere from (%.2f %.2f %.2f) to (%.2f %.2f %.2f)\n",
				mEntry.m_mSphere.m_vCenter.x, mEntry.m_mSphere.m_vCenter.y, mEntry.m_mSphere.m_vCenter.z,
				mEntry.m_mSphere.m_vCenter.x + fOffsetX, mEntry.m_mSphere.m_vCenter.y + fOffsetY, mEntry.m_mSphere.m_vCenter.z + fOffsetZ);

			mEntry.m_mSphere.m_vCenter.x += fOffsetX;
			mEntry.m_mSphere.m_vCenter.y += fOffsetY;
			mEntry.m_mSphere.m_vCenter.z += fOffsetZ;
			break;
	}
}

void MoveCarGenerator(IplCarsEntry& mEntry, float fOffsetX, float fOffsetY, float fOffsetZ) {
	printf("CARS: Changing car generator position from %.2f %.2f %.2f to %.2f %.2f %.2f\n",
		mEntry.m_vPosition.x, mEntry.m_vPosition.y, mEntry.m_vPosition.z,
		mEntry.m_vPosition.x + fOffsetX,
	    mEntry.m_vPosition.y + fOffsetY,
	    mEntry.m_vPosition.z + fOffsetZ);

	mEntry.m_vPosition.x += fOffsetX;
	mEntry.m_vPosition.y += fOffsetY;
	mEntry.m_vPosition.z += fOffsetZ;
}

void MoveCullZone(IplCullEntry& mEntry, float fOffsetX, float fOffsetY, float fOffsetZ) {
	// ...
}

void MoveEnex(IplEnexEntry& mEntry, float fOffsetX, float fOffsetY, float fOffsetZ) {
	printf("ENEX: Changing entrance/exit %s position from %.2f %.2f %.2f to %.2f %.2f %.2f\n",
		mEntry.m_szName,
		mEntry.m_vEntrancePos.x, mEntry.m_vEntrancePos.y, mEntry.m_vEntrancePos.z,
		mEntry.m_vEntrancePos.x + fOffsetX,
		mEntry.m_vEntrancePos.y + fOffsetY,
		mEntry.m_vEntrancePos.z + fOffsetZ);

	mEntry.m_vEntrancePos.x += fOffsetX;
	mEntry.m_vEntrancePos.y += fOffsetY;
	mEntry.m_vEntrancePos.z += fOffsetZ;
	mEntry.m_vExitPos.x += fOffsetX;
	mEntry.m_vExitPos.y += fOffsetY;
	mEntry.m_vExitPos.z += fOffsetZ;
}

void MoveGarage(IplGrgeEntry& mEntry, float fOffsetX, float fOffsetY, float fOffsetZ) {
	// ...
}

void MoveObjectInstance(IplInstEntry& mEntry, float fOffsetX, float fOffsetY, float fOffsetZ) {
	printf("INST: Changing %s position from %.2f %.2f %.2f to %.2f %.2f %.2f\n",
		mEntry.m_szModelName,
		mEntry.m_vPosition.x, mEntry.m_vPosition.y, mEntry.m_vPosition.z,
	    mEntry.m_vPosition.x + fOffsetX,
		mEntry.m_vPosition.y + fOffsetY,
		mEntry.m_vPosition.z + fOffsetZ);

	mEntry.m_vPosition.x += fOffsetX;
	mEntry.m_vPosition.y += fOffsetY;
	mEntry.m_vPosition.z += fOffsetZ;
}

void MoveJump(IplJumpEntry& mEntry, float fOffsetX, float fOffsetY, float fOffsetZ) {
	printf("JUMP: Changing stunt jump position\n");

	mEntry.m_mStartArea.m_vMin.x += fOffsetX;
	mEntry.m_mStartArea.m_vMin.y += fOffsetY;
	mEntry.m_mStartArea.m_vMin.z += fOffsetZ;
	mEntry.m_mStartArea.m_vMax.x += fOffsetX;
	mEntry.m_mStartArea.m_vMax.y += fOffsetY;
	mEntry.m_mStartArea.m_vMax.z += fOffsetZ;

	mEntry.m_mTargetArea.m_vMin.x += fOffsetX;
	mEntry.m_mTargetArea.m_vMin.y += fOffsetY;
	mEntry.m_mTargetArea.m_vMin.z += fOffsetZ;
	mEntry.m_mTargetArea.m_vMax.x += fOffsetX;
	mEntry.m_mTargetArea.m_vMax.y += fOffsetY;
	mEntry.m_mTargetArea.m_vMax.z += fOffsetZ;

	mEntry.m_vCameraPos.x += fOffsetX;
	mEntry.m_vCameraPos.y += fOffsetY;
	mEntry.m_vCameraPos.z += fOffsetZ;
}

void MoveOccluder(IplOcclEntry& mEntry, float fOffsetX, float fOffsetY, float fOffsetZ) {
	printf("OCCL: Changing occluder position %.2f %.2f %.2f to %.2f %.2f %.2f\n",
		mEntry.m_vCenter.x, mEntry.m_vCenter.y, mEntry.m_fBottom,
		mEntry.m_vCenter.x + fOffsetX,
		mEntry.m_vCenter.y + fOffsetY,
		mEntry.m_fBottom + fOffsetZ);

	mEntry.m_vCenter.x += fOffsetX;
	mEntry.m_vCenter.y += fOffsetY;
	mEntry.m_fBottom += fOffsetZ;
}

void MovePickup(IplPickEntry& mEntry, float fOffsetX, float fOffsetY, float fOffsetZ) {
	printf("PICK: Changing pickup position from %.2f %.2f %.2f to %.2f %.2f %.2f\n",
		mEntry.m_vPosition.x, mEntry.m_vPosition.y, mEntry.m_vPosition.z,
		mEntry.m_vPosition.x + fOffsetX,
		mEntry.m_vPosition.y + fOffsetY,
		mEntry.m_vPosition.z + fOffsetZ);

	mEntry.m_vPosition.x += fOffsetX;
	mEntry.m_vPosition.y += fOffsetY;
	mEntry.m_vPosition.z += fOffsetZ;
}

void MoveTimeCycleModifier(IplTcycEntry& mEntry, float fOffsetX, float fOffsetY, float fOffsetZ) {
	printf("TCYC: Changing time cycle modifier box from (%.2f %.2f %.2f - %.2f %.2f %.2f) to (%.2f %.2f %.2f - %.2f %.2f %.2f)\n",
		mEntry.m_mBox.m_vMin.x, mEntry.m_mBox.m_vMin.y, mEntry.m_mBox.m_vMin.z,
		mEntry.m_mBox.m_vMax.x, mEntry.m_mBox.m_vMax.y, mEntry.m_mBox.m_vMax.z,
		mEntry.m_mBox.m_vMin.x + fOffsetX, mEntry.m_mBox.m_vMin.y + fOffsetY, mEntry.m_mBox.m_vMin.z + fOffsetZ,
		mEntry.m_mBox.m_vMax.x + fOffsetX, mEntry.m_mBox.m_vMax.y + fOffsetY, mEntry.m_mBox.m_vMax.z + fOffsetZ);

	mEntry.m_mBox.m_vMin.x += fOffsetX;
	mEntry.m_mBox.m_vMin.y += fOffsetY;
	mEntry.m_mBox.m_vMin.z += fOffsetZ;
	mEntry.m_mBox.m_vMax.x += fOffsetX;
	mEntry.m_mBox.m_vMax.y += fOffsetY;
	mEntry.m_mBox.m_vMax.z += fOffsetZ;
}

void MoveZone(IplZoneEntry& mEntry, float fOffsetX, float fOffsetY, float fOffsetZ) {
	// ...
}

int main(int argc, char* argv[]) {
	WIN32_FIND_DATAA mFindFileData;
	float fOffsetX, fOffsetY, fOffsetZ;
	HANDLE hResult;

	enum {
		ARG_APP_NAME = 0,
		ARG_OFFSET_X,
		ARG_OFFSET_Y,
		ARG_OFFSET_Z,
		ARG_INPUT_FOLDER
	};

	if (argc < 4) {
		printf("Usage: %s [offset X] [offset Y] [offset Z]\n", argv[ARG_APP_NAME]);
		return 1;
	}

	if (!ValidateNumber(argv[ARG_OFFSET_X]) || !ValidateNumber(argv[ARG_OFFSET_Y]) || !ValidateNumber(argv[ARG_OFFSET_Z])) {
		printf("Error: Invalid numeric argument(s). All numeric arguments must be positive integers.\n");
		return 1;
	}

	fOffsetX = atof(argv[ARG_OFFSET_X]);
	fOffsetY = atof(argv[ARG_OFFSET_Y]);
	fOffsetZ = atof(argv[ARG_OFFSET_Z]);

	CreateDirectoryA("input", nullptr);
	CreateDirectoryA("output", nullptr);

	hResult = FindFirstFileA("input\\*.ipl", &mFindFileData);
	if (hResult == INVALID_HANDLE_VALUE) {
		printf("Error: No .ipl files found in input folder.\n");
		return 1;
	}

	do {
		char szInputPath[MAX_PATH];
		char szOutputPath[MAX_PATH];
		IplFile mIplFile;

		printf("Found IPL file: %s\n", mFindFileData.cFileName);

		sprintf(szInputPath, "input/%s", mFindFileData.cFileName);
		TRY_LOG_IF_FAILED(mIplFile.Read(szInputPath));

		for (auto& it : mIplFile.m_vAuzoEntries)
			MoveAudioZone(it, fOffsetX, fOffsetY, fOffsetZ);
		for (auto& it : mIplFile.m_vCarsEntries)
			MoveCarGenerator(it, fOffsetX, fOffsetY, fOffsetZ);
		for (auto& it : mIplFile.m_vCullEntries)
			MoveCullZone(it, fOffsetX, fOffsetY, fOffsetZ);
		for (auto& it : mIplFile.m_vEnexEntries)
			MoveEnex(it, fOffsetX, fOffsetY, fOffsetZ);
		for (auto& it : mIplFile.m_vGrgeEntries)
			MoveGarage(it, fOffsetX, fOffsetY, fOffsetZ);
		for (auto& it : mIplFile.m_vInstEntries)
			MoveObjectInstance(it, fOffsetX, fOffsetY, fOffsetZ);
		for (auto& it : mIplFile.m_vJumpEntries)
			MoveJump(it, fOffsetX, fOffsetY, fOffsetZ);
		for (auto& it : mIplFile.m_vOcclEntries)
			MoveOccluder(it, fOffsetX, fOffsetY, fOffsetZ);
		for (auto& it : mIplFile.m_vPickEntries)
			MovePickup(it, fOffsetX, fOffsetY, fOffsetZ);
		for (auto& it : mIplFile.m_vTcycEntries)
			MoveTimeCycleModifier(it, fOffsetX, fOffsetY, fOffsetZ);
		for (auto& it : mIplFile.m_vZoneEntries)
			MoveZone(it, fOffsetX, fOffsetY, fOffsetZ);

		sprintf(szOutputPath, "output/%s", mFindFileData.cFileName);
		TRY_LOG_IF_FAILED(mIplFile.Write(szOutputPath));

	} while (FindNextFileA(hResult, &mFindFileData) != 0);

	FindClose(hResult);

	return 0;
}