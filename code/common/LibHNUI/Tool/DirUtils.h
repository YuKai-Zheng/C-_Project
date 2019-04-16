#ifndef __DIR_H__
#define __DIR_H__
namespace HN
{
class DirUtils {
		
	public:
		
		DirUtils();
		DirUtils(DirUtils &dir);
		~DirUtils();
		
		DirUtils &operator=(DirUtils &file);
		
		// open/close
		int open(const char *name);
		int close();
		int isOpened() const;
		
		const char *getName() const;
		
		// directories
		int getNumDirs() const;
		const char *getDirName(int num) const;
		
		// files
		int getNumFiles() const;
		const char *getFileName(int num) const;
		
		// directories
		static const char *getHomeDir();
		static const char *getCurrentDir();
		static int chdir(const char *name);
		static int mkdir(const char *name);
		static int rmdir(const char *name);
		
		// remove file
		static int remove(const char *name);
		
		// rename file
		static int rename(const char *name,const char *new_name);
		
		// status
		static int isDir(const char *name);
		static int isFile(const char *name);
		static long long getMTime(const char *name);
		
	private:
		
		void add_directory(const char *name);
		void add_file(const char *name);
		
		struct DirData;		// dir implementation
		DirData *data;
};
}
#endif /* __DIR_H__ */
