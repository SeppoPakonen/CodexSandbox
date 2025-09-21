#ifndef _Core_Management_h_
#define _Core_Management_h_

NAMESPACE_SDK_BEGIN

struct UppAssemblyData;


class UppAssembly {
	Index<String> dirs;
	VectorMap<String, String> prj_paths;
	
	
public:
	UppAssembly();
	
	void AddPath(String dir);
	
	int GetDirectoryCount() const {return dirs.GetCount();}
	String GetDirectory(int i) const {return dirs[i];}
	int FindUppProject(String name) const {return prj_paths.Find(name);}
	String GetUppProjectPath(int i) const {return prj_paths[i];}
	
	void Dump() {DUMPC(dirs); DUMPM(prj_paths);}
};


class UppProject {
	String name, path, dir;
	
	Index<String> uses;
	Vector<String> files;
	VectorMap<String, String> configs;
	
public:
	
	String GetUse(int i) const {return uses[i];}
	int GetUseCount() const {return uses.GetCount();}
	String GetFile(int i) const {return files[i];}
	int GetFileCount() const {return files.GetCount();}
	String GetConfigKey(int i) const {return configs.GetKey(i);}
	String GetConfigValue(int i) const {return configs[i];}
	int GetConfigCount() const {return configs.GetCount();}
	String GetFilePath() const {return path;}
	String GetFilePath(int i) const {return AppendFileName(dir, files[i]);}
	String GetName() const {return name;}
	void AddFile(String filename) {files.Add(filename);}
	String GetDirectory() const {return dir;}
	void ClearFiles() {files.Clear();}
	void RemoveFile(int i) {files.Remove(i);}
	
	void Clear();
	void Store();
	bool Load(String name, String path);
	void Refresh() {Load(name, path);}
	void GetRecursiveUses(Index<String>& idx, UppAssemblyData& as);
	
	void Dump() {DUMP(path); DUMP(dir); DUMPC(uses); DUMPC(files); DUMPM(configs);}
};


struct UppAssemblyData {
	const UppAssembly& as;
	ArrayMap<String, UppProject> prjs;
	Mutex lock;
	
	
	UppAssemblyData(const UppAssembly& as) : as(as) {}
	UppProject& GetUppProject(String name);
	
};


NAMESPACE_SDK_END

#endif
