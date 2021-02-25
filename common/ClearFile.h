#ifndef _CLEARFILE_H_
#define _CLEARFILE_H_

#include <iostream>

using namespace std;

class ClearFile
{
public:
	ClearFile(string);

private:
	void DeleteAllFiles(string strPath);
	inline bool is_special_dir(const char *path);
	inline bool is_dir(int attrib);
	inline void get_file_path(const char *path, const char *file_name, char *file_path);
	inline void show_error(const char *file_name);


};




#endif // !1
