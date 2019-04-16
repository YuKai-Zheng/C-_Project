#include "cocos2d.h"



#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	#include <windows.h>
	#include <sys/stat.h>
	#include <direct.h>
	#include <io.h>
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX || CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	#include <unistd.h>
	#include <dirent.h>
	#include <sys/stat.h>
#endif

#include "DirUtils.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#define PATH_PREFIX	""
#endif


using namespace HN;

/*
 */
struct DirUtils::DirData {
	char *name;
	int num_dirs;
	char **dirs;
	int num_files;
	char **files;
};

/*
 */
DirUtils::DirUtils() {
	data = new DirData();
	data->name = NULL;
	data->num_dirs = 0;
	data->dirs = NULL;
	data->num_files = 0;
	data->files = NULL;
}

DirUtils::DirUtils(DirUtils &dir) {
	data = new DirData();
	data->name = dir.data->name;
	data->num_dirs = dir.data->num_dirs;
	data->dirs = dir.data->dirs;
	data->num_files = dir.data->num_files;
	data->files = dir.data->files;
	dir.data->name = NULL;
	dir.data->num_dirs = 0;
	dir.data->dirs = NULL;
	dir.data->num_files = 0;
	dir.data->files = NULL;
}

DirUtils::~DirUtils() {
	CC_ASSERT(data->name == NULL && "Dir::~Dir(): directory is not closed");
	delete data;
}

/*
 */
DirUtils &DirUtils::operator=(DirUtils &dir) {
	if(this == &dir) return *this;
	CC_ASSERT(data->name == NULL && "Dir::operator=(): directory is not closed");
	data->name = dir.data->name;
	data->num_dirs = dir.data->num_dirs;
	data->dirs = dir.data->dirs;
	data->num_files = dir.data->num_files;
	data->files = dir.data->files;
	dir.data->name = NULL;
	dir.data->num_dirs = 0;
	dir.data->dirs = NULL;
	dir.data->num_files = 0;
	dir.data->files = NULL;
	return *this;
}

/*
 */
void DirUtils::add_directory(const char *name) {
	
	if(data->num_dirs % 256 == 0) {
		char **dirs = data->dirs;
		data->dirs = new char*[data->num_dirs + 256];
		for(int i = 0; i < data->num_dirs; i++) {
			data->dirs[i] = dirs[i];
		}
		delete [] dirs;
	}
	
	data->dirs[data->num_dirs] = new char[strlen(name) + 1];
	strcpy(data->dirs[data->num_dirs],name);
	data->num_dirs++;
}

void DirUtils::add_file(const char *name) {
	
	if(data->num_files % 256 == 0) {
		char **files = data->files;
		data->files = new char*[data->num_files + 256];
		for(int i = 0; i < data->num_files; i++) {
			data->files[i] = files[i];
		}
		delete [] files;
	}
	
	data->files[data->num_files] = new char[strlen(name) + 1];
	strcpy(data->files[data->num_files],name);
	data->num_files++;
}

/*
 */
int DirUtils::open(const char *name) {
	
	CC_ASSERT(data->name == NULL && "Dir::open(): directory is already opened");
	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		
		char buf[1024];
		char file[1024];
		wchar_t wbuf[1024];
		
		_snprintf(buf,sizeof(buf),"%s*",name);
		buf[sizeof(buf) - 1] = '\0';
		MultiByteToWideChar(CP_UTF8,0,buf,-1,wbuf,sizeof(wbuf) / sizeof(wbuf[0]));
		wbuf[sizeof(wbuf) / sizeof(wbuf[0]) - 1] = '\0';
		
		struct _wfinddata_t fileinfo;
		intptr_t handle = _wfindfirst(wbuf,&fileinfo);
		if(handle == -1) return 0;
		
		do {
			if(!wcscmp(fileinfo.name,L".") || !wcscmp(fileinfo.name,L"..")) continue;
			WideCharToMultiByte(CP_UTF8,0,fileinfo.name,-1,file,sizeof(file),NULL,NULL);
			file[sizeof(file) - 1] = '\0';
			_snprintf(buf,sizeof(buf),"%s%s",name,file);
			buf[sizeof(buf) - 1] = '\0';
			if(fileinfo.attrib & _A_SUBDIR) {
				strcat(buf,"/");
				add_directory(buf);
			} else {
				add_file(buf);
			}
		} while(_wfindnext(handle,&fileinfo) != -1);
		
		_findclose(handle);
		
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX || CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		
		char buf[1024];
		struct stat st;
		struct dirent *dirent;
		
		DIR *dir = opendir(name);
		if(dir == NULL) return 0;
		
		while((dirent = readdir(dir)) != NULL) {
			if(!strcmp(dirent->d_name,".") || !strcmp(dirent->d_name,"..")) continue;
			snprintf(buf,sizeof(buf),"%s%s",name,dirent->d_name);
			buf[sizeof(buf) - 1] = '\0';
			if(stat(buf,&st)) continue;
			if(st.st_mode & S_IFDIR) {
				strcat(buf,"/");
				add_directory(buf);
			} else if(st.st_mode & S_IFREG) {
				add_file(buf);
			}
		}
		
		closedir(dir);
		
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		
		char buf[1024];
		struct stat st;
		struct dirent *dirent;
		
		snprintf(buf,sizeof(buf),PATH_PREFIX"%s",name);
		buf[sizeof(buf) - 1] = '\0';
		DIR *dir = opendir(buf);
		if(dir == NULL) return 0;
		
		size_t length = strlen(PATH_PREFIX);
		while((dirent = readdir(dir)) != NULL) {
			if(!strcmp(dirent->d_name,".") || !strcmp(dirent->d_name,"..")) continue;
			snprintf(buf,sizeof(buf),PATH_PREFIX"%s%s",name,dirent->d_name);
			buf[sizeof(buf) - 1] = '\0';
			if(stat(buf,&st)) continue;
			if(st.st_mode & S_IFDIR) {
				strcat(buf,"/");
				add_directory(buf + length);
			} else if(st.st_mode & S_IFREG) {
				add_file(buf + length);
			}
		}
		
		closedir(dir);
		
#endif
	
	data->name = new char[strlen(name) + 1];
	strcpy(data->name,name);
	
	return 1;
}

int DirUtils::close() {
	
	CC_ASSERT(data->name != NULL && "Dir::close(): directory is not opened");
	
	delete [] data->name;
	data->name = NULL;
	
	for(int i = 0; i < data->num_dirs; i++) {
		delete [] data->dirs[i];
	}
	delete [] data->dirs;
	data->num_dirs = 0;
	data->dirs = NULL;
	
	for(int i = 0; i < data->num_files; i++) {
		delete [] data->files[i];
	}
	delete [] data->files;
	data->num_files = 0;
	data->files = NULL;
	
	return 1;
}

/*
 */
int DirUtils::isOpened() const {
	return (data->name != NULL);
}

const char *DirUtils::getName() const {
	CC_ASSERT(data->name != NULL && "Dir::getName(): directory is not opened");
	return data->name;
}

/*
 */
int DirUtils::getNumDirs() const {
	CC_ASSERT(data->name != NULL && "Dir::getNumDirs(): directory is not opened");
	return data->num_dirs;
}

const char *DirUtils::getDirName(int num) const {
	CC_ASSERT(data->name != NULL && "Dir::getDirName(): directory is not opened");
	CC_ASSERT(num >= 0 && num < data->num_dirs && "Dir::getDirName(): bad directory number");
	return data->dirs[num];
}

/*
 */
int DirUtils::getNumFiles() const {
	CC_ASSERT(data->name != NULL && "Dir::getNumFiles(): directory is not opened");
	return data->num_files;
}

const char *DirUtils::getFileName(int num) const {
	CC_ASSERT(data->name != NULL && "Dir::getFileName(): directory is not opened");
	CC_ASSERT(num >= 0 && num < data->num_files && "Dir::getFileName(): bad file number");
	return data->files[num];
}

/******************************************************************************\
*
* static functions
*
\******************************************************************************/

/*
 */
const char *DirUtils::getHomeDir() {
	static char buf[1024];
	memset(buf,0,sizeof(buf));
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		const wchar_t *userprofile = _wgetenv(L"USERPROFILE");
		if(userprofile != NULL) {
			WideCharToMultiByte(CP_UTF8,0,userprofile,-1,buf,sizeof(buf),NULL,NULL);
			if(buf[0]) buf[0] = toupper(buf[0]);
			buf[sizeof(buf) - 1] = '\0';
			for(char *s = buf; *s != '\0'; s++) {
				if(*s == '\\') *s = '/';
			}
		}
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX || CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		const char *home = getenv("HOME");
		if(home != NULL) {
			strncpy(buf,home,sizeof(buf));
			buf[sizeof(buf) - 1] = '\0';
		}
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		strcpy(buf,PATH_PREFIX);
	#endif
	return buf;
}

const char *DirUtils::getCurrentDir() {
	static char buf[1024];
	memset(buf,0,sizeof(buf));
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		static wchar_t wbuf[1024];
		if(_wgetcwd(wbuf,sizeof(wbuf) / sizeof(wbuf[0])) != NULL) {
			wbuf[sizeof(wbuf) / sizeof(wbuf[0]) - 1] = '\0';
			WideCharToMultiByte(CP_UTF8,0,wbuf,-1,buf,sizeof(buf),NULL,NULL);
			if(buf[0]) buf[0] = toupper(buf[0]);
			buf[sizeof(buf) - 1] = '\0';
			for(char *s = buf; *s != '\0'; s++) {
				if(*s == '\\') *s = '/';
			}
		}
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX || CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		if(getcwd(buf,sizeof(buf)) != NULL) {
			buf[sizeof(buf) - 1] = '\0';
		}
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		strcpy(buf,PATH_PREFIX);
	#endif
	return buf;
}

int DirUtils::chdir(const char *name) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		wchar_t wbuf[1024];
		MultiByteToWideChar(CP_UTF8,0,name,-1,wbuf,sizeof(wbuf) / sizeof(wbuf[0]));
		wbuf[sizeof(wbuf) / sizeof(wbuf[0]) - 1] = '\0';
		return (_wchdir(wbuf) == 0);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX || CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		return (::chdir(name) == 0);
#else
		return 0;
#endif
}

int DirUtils::mkdir(const char *name) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		wchar_t wbuf[1024];
		MultiByteToWideChar(CP_UTF8,0,name,-1,wbuf,sizeof(wbuf) / sizeof(wbuf[0]));
		wbuf[sizeof(wbuf) / sizeof(wbuf[0]) - 1] = '\0';
		return (_wmkdir(wbuf) == 0);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX || CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		return (::mkdir(name,0755) == 0);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		char buf[1024];
		snprintf(buf,sizeof(buf),PATH_PREFIX"%s",name);
		buf[sizeof(buf) - 1] = '\0';
		return (::mkdir(buf,0755) == 0);
#else
		return 0;
#endif
}

int DirUtils::rmdir(const char *name) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		wchar_t wbuf[1024];
		MultiByteToWideChar(CP_UTF8,0,name,-1,wbuf,sizeof(wbuf) / sizeof(wbuf[0]));
		wbuf[sizeof(wbuf) / sizeof(wbuf[0]) - 1] = '\0';
		int nRet = _wrmdir(wbuf);
		return (nRet == 0);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX || CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		return (::rmdir(name) == 0);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		char buf[1024];
		snprintf(buf,sizeof(buf),PATH_PREFIX"%s",name);
		buf[sizeof(buf) - 1] = '\0';
		return (::rmdir(buf) == 0);
#else
		return 0;
#endif
}

/*
 */
int DirUtils::remove(const char *name) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		wchar_t wbuf[1024];
		MultiByteToWideChar(CP_UTF8,0,name,-1,wbuf,sizeof(wbuf) / sizeof(wbuf[0]));
		wbuf[sizeof(wbuf) / sizeof(wbuf[0]) - 1] = '\0';
		return (_wremove(wbuf) == 0);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX || CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		return (::remove(name) == 0);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		char buf[1024];
		snprintf(buf,sizeof(buf),PATH_PREFIX"%s",name);
		buf[sizeof(buf) - 1] = '\0';
		return (::remove(buf) == 0);
#else
		return 0;
#endif
}

int DirUtils::rename(const char *name,const char *new_name) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		wchar_t wbuf[1024];
		wchar_t new_wbuf[1024];
		MultiByteToWideChar(CP_UTF8,0,name,-1,wbuf,sizeof(wbuf) / sizeof(wbuf[0]));
		MultiByteToWideChar(CP_UTF8,0,new_name,-1,new_wbuf,sizeof(new_wbuf) / sizeof(new_wbuf[0]));
		wbuf[sizeof(wbuf) / sizeof(wbuf[0]) - 1] = '\0';
		new_wbuf[sizeof(new_wbuf) / sizeof(new_wbuf[0]) - 1] = '\0';
		return (_wrename(wbuf,new_wbuf) == 0);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX || CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		return (::rename(name,new_name) == 0);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		char buf[1024];
		char new_buf[1024];
		snprintf(buf,sizeof(buf),PATH_PREFIX"%s",name);
		snprintf(new_buf,sizeof(new_buf),PATH_PREFIX"%s",new_name);
		buf[sizeof(buf) - 1] = '\0';
		new_buf[sizeof(new_buf) - 1] = '\0';
		return (::rename(buf,new_buf) == 0);
#else
		return 0;
#endif
}

/*
 */
int DirUtils::isDir(const char *name) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		struct _stat st;
		wchar_t wbuf[1024];
		MultiByteToWideChar(CP_UTF8,0,name,-1,wbuf,sizeof(wbuf) / sizeof(wbuf[0]));
		wbuf[sizeof(wbuf) / sizeof(wbuf[0]) - 1] = '\0';
		size_t length = wcslen(wbuf);
		if(length > 0 && (wbuf[length - 1] == '/' || wbuf[length - 1] == '\\')) wbuf[length - 1] = '\0';
		if(_wstat(wbuf,&st)) return 0;
		if(st.st_mode & _S_IFDIR) return 1;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX || CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		struct stat st;
		if(stat(name,&st)) return 0;
		if(st.st_mode & S_IFDIR) return 1;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		struct stat st;
		char buf[1024];
		snprintf(buf,sizeof(buf),PATH_PREFIX"%s",name);
		buf[sizeof(buf) - 1] = '\0';
		if(stat(buf,&st)) return 0;
		if(st.st_mode & S_IFDIR) return 1;
#endif
	return 0;
}

int DirUtils::isFile(const char *name) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		struct _stat st;
		wchar_t wbuf[1024];
		MultiByteToWideChar(CP_UTF8,0,name,-1,wbuf,sizeof(wbuf) / sizeof(wbuf[0]));
		wbuf[sizeof(wbuf) / sizeof(wbuf[0]) - 1] = '\0';
		if(_wstat(wbuf,&st)) return 0;
		if(st.st_mode & _S_IFREG) return 1;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX || CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		struct stat st;
		if(stat(name,&st)) return 0;
		if(st.st_mode & S_IFREG) return 1;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		struct stat st;
		char buf[1024];
		snprintf(buf,sizeof(buf),PATH_PREFIX"%s",name);
		buf[sizeof(buf) - 1] = '\0';
		if(stat(buf,&st)) return 0;
		if(st.st_mode & S_IFREG) return 1;
#endif
	return 0;
}

long long DirUtils::getMTime(const char *name) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		struct _stat st;
		wchar_t wbuf[1024];
		MultiByteToWideChar(CP_UTF8,0,name,-1,wbuf,sizeof(wbuf) / sizeof(wbuf[0]));
		wbuf[sizeof(wbuf) / sizeof(wbuf[0]) - 1] = '\0';
		if(_wstat(wbuf,&st)) return -1;
		return (long long)st.st_mtime;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX || CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		struct stat st;
		if(stat(name,&st)) return -1;
		return (long long)st.st_mtime;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		struct stat st;
		char buf[1024];
		snprintf(buf,sizeof(buf),PATH_PREFIX"%s",name);
		buf[sizeof(buf) - 1] = '\0';
		if(stat(buf,&st)) return -1;
		return (long long)st.st_mtime;
#else
		return -1;
#endif
}
