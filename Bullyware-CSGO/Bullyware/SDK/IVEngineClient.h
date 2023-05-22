#pragma once

typedef struct player_info_s
{
	// version for future compatibility
	unsigned __int64			version;
	// network xuid
	unsigned __int64			xuid;
	// scoreboard information
	char			name[32];
	// local server user ID, unique while server is running
	int				userID;
	// global unique player identifer
	char			guid[32 + 1];
	// friends identification number
	unsigned __int32			friendsID;
	// friends name
	char			friendsName[32];
	// true, if player is a bot controlled by game.dll
	bool			fakeplayer;
	// true if player is the HLTV proxy
	bool			ishltv;
#if defined( REPLAY_ENABLED )
	// true if player is the Replay proxy
	bool			isreplay;
#endif
	// custom files CRC for this player
	void* customFiles[32];
	// this counter increases each time the server downloaded a new file
	unsigned char	filesDownloaded;
} player_info_t;


class IVEngineClient
{
public:
	virtual int					GetIntersectingSurfaces(
		const void** model,
		const void*& vCenter,
		const float radius,
		const bool bOnlyVisibleSurfaces,	// Only return surfaces visible to vCenter.
		void** pInfos,
		const int nMaxInfos) = 0; // 0

	virtual void* GetLightForPoint(const void*& pos, bool bClamp) = 0; // 1
	virtual void* TraceLineMaterialAndLighting(const void*& start, const void*& end, // 2
		void*& diffuseLightColor, void*& baseColor) = 0; // 3
	virtual const char* ParseFile(const char* data, char* token, int maxlen) = 0; // 4
	virtual bool				CopyLocalFile(const char* source, const char* destination) = 0; // 5
	virtual void				GetScreenSize(int& width, int& height) = 0; // 6
	virtual void				ServerCmd(const char* szCmdString, bool bReliable = true) = 0; // 7
	virtual void				ClientCmd(const char* szCmdString) = 0; // 8
	virtual bool				GetPlayerInfo(int ent_num, player_info_t* pinfo) = 0; // 9
	virtual int					GetPlayerForUserID(int userID) = 0; // 10
	virtual void* TextMessageGet(const char* pName) = 0; // 11
	virtual bool				Con_IsVisible(void) = 0; // 12
	virtual int					GetLocalPlayer(void) = 0; // 13
};