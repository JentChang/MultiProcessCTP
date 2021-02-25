#ifndef BACKTRADERCN_SYSTEMDIR_H
#define BACKTRADERCN_SYSTEMDIR_H

#include <filesystem>
#include <string>

//in windows folder
#ifdef _WINDOWS
#define BackTraderCN_FOLDER "D:/BackTraderCN/"                 /**< base folder of BackTraderCN system */
#else
//in linux folder
#define BackTraderCN_FOLDER "/BackTraderCN/"
#endif

typedef std::pair<std::string, std::string> JournalPair;

class SystemDir {

private:
    SystemDir();
    static std::string folder;

public:
    //set folder when the world start
    static inline void setFolder(const std::string& dir)
    {
        folder = dir;
    };
    static inline std::string getFolder()
    {
        return getDirNotEndWithPathSep(folder);
    };
    static inline std::string getRuntimeFolder()
    {
        return getDirNotEndWithPathSep(folder) + "/runtime/";
    };

    static inline std::string getGatewayDir()
    {
        return getDirNotEndWithPathSep(folder) + "/gateway";
    };

    static inline int getPageEngineListenPort()
    {
        return 8118;
    };

    // directory should not contain '/' in the back
    static inline std::string getDirNotEndWithPathSep(const std::string& _dir)
    {
        if (_dir.back() == '/')
            return _dir.substr(0, _dir.length() - 1);
        else
            return _dir;
    }

	static inline std::string getJournalDir()
	{
		return getDirNotEndWithPathSep(folder) + "/journal/";
	};

	static inline std::string getGatewayStateDbDir()
	{
		return getDirNotEndWithPathSep(folder) + "/gateway/state/";
	};

	static inline std::string getLogDir()
	{
		return getDirNotEndWithPathSep(folder) + "/log/";
	};
	
	static inline std::string getStrategyLogDir()
	{
		return getDirNotEndWithPathSep(folder) + "/log/strategy/";
	};

	static inline JournalPair getSysJournalPair()
	{
		return {getDirNotEndWithPathSep(folder) + "/journal/system/", "SYSTEM" };
	};

	static inline std::string getStrategyJournalDir()
	{
		return getJournalDir() + "strategy/";
	};

	static inline std::string getStrategyReplayJournalDir()
	{
		return getJournalDir() + "strategy_replay/";
	};

	static inline std::string getMdJournalFolder(const std::string& source)
	{
		return getDirNotEndWithPathSep(folder) + "/MD/SOURCE" + source + "/";
	};
	
	static inline std::string getMdJournalName(const std::string& source)
	{
		return "MD_" + source;
	};

	static inline std::string getTdJournalFolder(const std::string& source)
	{
		return getDirNotEndWithPathSep(folder) + "/TD/SOURCE" + source + "/";
	};

	static inline std::string getTdJournalName(const std::string& source)
	{
		return "TD_" + source;
	};

	static inline std::string getMdSubscriptionDbFile(const std::string& name)
	{
		return getDirNotEndWithPathSep(folder) + "/MD/db/" + name + ".db";
	};

	//in the Journal there is a map(JournalReader::addJournal:  journalMap[jname]) , the key is a filename , so the file name should be unique.
    static inline JournalPair getMdJournalPair(short source)
    {
        return {getDirNotEndWithPathSep(folder) + "/journal/SOURCE" + std::to_string(source) + "/MD/", "MD_SOURCE" + std::to_string(source)};
    };

    static inline JournalPair getMdRawJournalPair(short source)
    {
        return {getDirNotEndWithPathSep(folder) + "/journal/SOURCE" + std::to_string(source) + "/MD_RAW/", "MD_RAW_SOURCE" + std::to_string(source)};
    };

    static inline JournalPair getTdJournalPair(short source)
    {
        return {getDirNotEndWithPathSep(folder) + "/journal/SOURCE" + std::to_string(source) + "/TD/", "TD_SOURCE" + std::to_string(source)};
    };

    static inline JournalPair getTdSendJournalPair(short source)
    {
        return {getDirNotEndWithPathSep(folder) + "/journal/SOURCE" + std::to_string(source) + "/TD_SEND/", "TD_SEND_SOURCE" + std::to_string(source)};
    };

    static inline JournalPair getTdRawJournalPair(short source)
    {
        return {getDirNotEndWithPathSep(folder) + "/journal/SOURCE" + std::to_string(source) + "/TD_RAW/", "TD_RAW_SOURCE" + std::to_string(source)};
    };

	static inline JournalPair getTdQJournalPair(short source)
    {
        return {getDirNotEndWithPathSep(folder) + "/journal/SOURCE" + std::to_string(source) + "/TD_Q/", "TD_Q_SOURCE" + std::to_string(source)};
    };

	static inline bool create_folder_if_not_exists(const std::string& path)
    {
        std::experimental::filesystem::path _path(path);
        if (!std::experimental::filesystem::exists(_path))
        {
            return std::experimental::filesystem::create_directories(_path);
        }
        else
        {
            return std::experimental::filesystem::is_directory(_path);
        }
    }
};


#endif //BACKTRADERCN_SYSTEMDIR_H
