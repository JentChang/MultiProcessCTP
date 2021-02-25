#include "ClearFile.h"

ClearFile::ClearFile(string)
{
	strPath += "\\*";
	this->DeleteAllFiles(strPath);
}

// 删除文件夹下的文件
void ClearFile::DeleteAllFiles(string strPath)
{
	_finddata_t dir_info;  // 文件夹信息
	_finddata_t file_info;  // 文件信息
	intptr_t f_handle;

	char tmp_path[_MAX_PATH];

	if ((f_handle = _findfirst(strPath.c_str(), &dir_info)) != -1)
	{
		while ((_findnext(f_handle, &file_info)) == 0)
		{
			if (is_special_dir(file_info.name))
				continue;
			if (is_dir(file_info.attrib))   //如果是目录，生成完整的路径
			{
				get_file_path(strPath.c_str(), file_info.name, tmp_path);
				ClearFile::DeleteAllFiles(tmp_path);    //开始递归删除目录中的内容
				tmp_path[strlen(tmp_path) - 2] = '\0';
				if (file_info.attrib == 20)
					printf("This is system file, can't delete!\n");
				else
				{
					// 删除空目录,必须在递归返回前调用 _findclose, 否则无法删除目录
					if (_rmdir(tmp_path) == -1)
						show_error();//目录非空则会显示出错原因
				}
			}
			else
			{
				strcpy_s(tmp_path, strPath.c_str());
				tmp_path[strlen(tmp_path) - 1] = '\0';
				strcat_s(tmp_path, file_info.name);  // 生成完整文件路径

				if (remove(tmp_path) == -1)
				{
					show_error(file_info.name);
				}
			}
		}
		_findclose(f_handle);//关闭打开的文件句柄，并释放关联资源，否则无法删除空目录
	}
	else
	{
		show_error();//若路径不存在，显示错误信息
	}
}

//判断是否是".."目录和"."目录
inline bool ClearFile::is_special_dir(const char *path)
{
	return strcmp(path, "..") == 0 || strcmp(path, ".") == 0;
}


//判断文件属性是目录还是文件
inline bool ClearFile::is_dir(int attrib)
{
	return attrib == 16 || attrib == 18 || attrib == 20;
}


inline void ClearFile::get_file_path(const char *path, const char *file_name, char *file_path)
{
	strcpy_s(file_path, sizeof(char) * _MAX_PATH, path);
	file_path[strlen(file_path) - 1] = '\0';
	strcat_s(file_path, sizeof(char) * _MAX_PATH, file_name);
	strcat_s(file_path, sizeof(char) * _MAX_PATH, "\\*");
}


//显示删除失败原因
inline void ClearFile::show_error(const char *file_name)
{
	errno_t err;
	_get_errno(&err);
	switch (err)
	{
	case ENOTEMPTY:
		printf("Given path is not a directory, the directory is not empty, or the directory is either the current working directory or the root directory.\n");
		break;
	case ENOENT:
		printf("Path is invalid.\n");
		break;
	case EACCES:
		printf("%s had been opend by some application, can't delete.\n", file_name);
		break;
	}
}