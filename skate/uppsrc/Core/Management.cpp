#include "Core.h"

NAMESPACE_SDK_BEGIN



UppAssembly::UppAssembly() {
	
}

void UppAssembly::AddPath(String dir) {
	if (dirs.Find(dir) >= 0) return;
	dirs.Add(dir);
	
	Index<String> files;
	GetDirectoryFiles(dir, files);
	
	for(int i = 0; i < files.GetCount(); i++) {
		String name = files[i];
		String path = AppendFileName(dir, name);
		//DUMP(path);
		if (DirectoryExists(path)) {
			String upp_file = AppendFileName(path, name + ".upp");
			if (FileExists(upp_file)) {
				prj_paths.Add(name, upp_file);
			}
		}
	}
	
	
	/*FindFile ff;
	
	if (ff.Search(AppendFileName(dir, "*"))) {
		do {
			if (ff.IsDirectory()) {
				String upp_file = AppendFileName(ff.GetPath(), ff.GetName() + ".upp");
				if (FileExists(upp_file)) {
					prj_paths.Add(ff.GetName(), upp_file);
				}
			}
		}
		while (ff.Next());
	}*/
}





void UppProject::Clear() {
	name = ""; path = ""; dir = "";
	uses.Clear();
	files.Clear();
	configs.Clear();
}

void UppProject::Store() {
	if (path.IsEmpty()) return;
	
	RealizeDirectory(dir);
	DUMP(path);
	
	FileOut upp_out(path);
	
	if (uses.GetCount()) {
		upp_out << "uses\n";
		for(int i = 0; i < uses.GetCount(); i++)
			upp_out << "\t" << uses[i] << (i < uses.GetCount()-1 ? "," : ";") << "\n";
	}
	
	if (files.GetCount()) {
		upp_out << "file\n";
		for(int i = 0; i < files.GetCount(); i++)
			upp_out << "\t" << files[i] << (i < files.GetCount()-1 ? "," : ";") << "\n";
	}
	
	if (configs.GetCount()) {
		upp_out << "mainconfig\n";
		for(int i = 0; i < configs.GetCount(); i++)
			upp_out << "\t\"" << configs.GetKey(i) << "\" = \"" << configs[i] << "\"" << (i < configs.GetCount()-1 ? ',' : ';') << "\n";
	}
	
}

bool UppProject::Load(String name, String path) {
	Clear();
	this->name = name;
	this->path = path;
	this->dir = GetFileDirectory(path);
	
	if (!FileExists(path)) return false;
	
	String content = LoadFile(path);
	
	Vector<String> lines = Split(content, "\n");
	
	int mode = -1;
	enum {USES, FILES, CONFIGS};
	for(int i = 0; i < lines.GetCount(); i++) {
		String line = lines[i];
		bool intended = line.Left(1) == "\t";
		
		line = TrimBoth(line);
		LOG(i << ": " << line);
		
		if (line == "uses")
			mode = USES;
		else if (line == "file")
			mode = FILES;
		else if (line == "mainconfig")
			mode = CONFIGS;
		else if (intended) {
			if (line.IsEmpty()) continue;
			line = line.Left(line.GetCount()-1); // ,
			int j = line.ReverseFind(" highlight ");
			if (j >= 0)
				line = line.Left(j);
			//DUMP(line);
			if (line.IsEmpty()) continue;
			if (mode == USES)
				uses.FindAdd(line);
			else if (mode == FILES)
				files.Add(line);
			else if (mode == CONFIGS) {
				String key, value;
				try {
					int j = line.Find("=");
					if (j < 0)
						throw Exc("Not found char '='");
					
					key = TrimBoth(line.Left(j));
					if (key.GetCount() >= 2 && key.Left(1) == "\"" && key.Right(1) == "\"")
						key = key.Mid(1, key.GetCount()-2);
					
					value = TrimBoth(line.Mid(j+1));
					if (value.GetCount() >= 2 && value.Left(1) == "\"" && value.Right(1) == "\"")
						value = value.Mid(1, value.GetCount()-2);
					
					configs.Add(key, value);
				}
				catch (Exc e) {
					LOG(e);
				}
			}
		}
	}
	
	return true;
}

void UppProject::GetRecursiveUses(Index<String>& idx, UppAssemblyData& as) {
	if (idx.Find(name) >= 0) return;
	idx.Add(name);
	for(int i = 0; i < uses.GetCount(); i++) {
		UppProject& prj = as.GetUppProject(uses[i]);
		prj.GetRecursiveUses(idx, as);
	}
}





UppProject& UppAssemblyData::GetUppProject(String name) {
	int i = as.FindUppProject(name);
	String upp_path;
	if (i >= 0) {
		upp_path = as.GetUppProjectPath(i);
	}
	else {
		ASSERT(as.GetDirectoryCount() > 0);
		String dir = as.GetDirectory(0);
		upp_path = AppendFileName(dir, name + DIR_SEPS + name + ".upp");
	}
	
	lock.Enter();
	i = prjs.Find(upp_path);
	if (i == -1) {
		LOG("Load " << upp_path);
		i = prjs.GetCount();
		prjs.Add(upp_path).Load(name, upp_path);
	}
	UppProject& prj = prjs[i];
	lock.Leave();
	return prj;
}


NAMESPACE_SDK_END
