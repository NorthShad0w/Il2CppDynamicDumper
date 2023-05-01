#pragma once

namespace PacketSniffer
{
	inline Il2CppClass* GetClassFromType(const Il2CppType* type)
	{
		static auto il2cpp_class_from_type = reinterpret_cast<Il2CppClass * (*)(const Il2CppType*)>(GetProcAddress(GetModuleHandleW(L"GameAssembly.dll"), "il2cpp_class_from_type"));
		return il2cpp_class_from_type(type);
	}

	inline int GetClassCountFromImage(Il2CppImage* i)
	{
		static auto il2cpp_image_get_class_count = reinterpret_cast<int(*)(Il2CppImage*)>(GetProcAddress(GetModuleHandleW(L"GameAssembly.dll"), "il2cpp_image_get_class_count"));
		return il2cpp_image_get_class_count(i);
	}

	inline Il2CppClass* GetClassFromImage(Il2CppImage* i, int index)
	{
		static auto il2cpp_image_get_class = reinterpret_cast<Il2CppClass * (*)(Il2CppImage*, int)>(GetProcAddress(GetModuleHandleW(L"GameAssembly.dll"), "il2cpp_image_get_class"));
		return il2cpp_image_get_class(i, index);
	}

	inline Il2CppAssembly** GetAssembliesFromDomain(Il2CppDomain* domain, size_t* size)
	{
		static auto il2cpp_domain_get_assemblies = reinterpret_cast<Il2CppAssembly * *(*)(Il2CppDomain*, size_t*)>(GetProcAddress(GetModuleHandleW(L"GameAssembly.dll"), "il2cpp_domain_get_assemblies"));
		return il2cpp_domain_get_assemblies(domain, size);
	}

	inline std::string GetMethodFlags(int f)
	{
		std::string flags = "";
		if (f & 0x0001) flags += "Private | ";			// 私有
		if (f & 0x0002) flags += "Famandassem | ";		// 保护和内部
		if (f & 0x0003) flags += "Assembly | ";			// 内部
		if (f & 0x0004) flags += "Family | ";			// 保护
		if (f & 0x0005) flags += "Famorassem | ";		// 保护或内部
		if (f & 0x0006) flags += "Public | ";			// 公开
		if (f & 0x0010) flags += "Static | ";			// 静态
		if (f & 0x0020) flags += "Final | ";			// 最终
		if (f & 0x0040) flags += "Virtual | ";			// 虚拟
		if (f & 0x0080) flags += "Hidebysig | ";		// 隐藏
		if (f & 0x0100) flags += "Newslot | ";			// 新插槽
		if (f & 0x0200) flags += "Strict | ";			// 严格
		if (f & 0x0400) flags += "Abstract | ";			// 抽象
		if (f & 0x0800) flags += "Specialname | ";		// 特殊名称
		if (f & 0x2000) flags += "Pinvokeimpl | ";
		if (f & 0x0008) flags += "Unmanagedexport | ";
		if (flags.length() > 0)
		{
			flags.pop_back();
			flags.pop_back();
		}
		return flags;
	}

	inline bool IsMethodStatic(const MethodInfo* method)
	{
		return (method->flags & 0x0010) == 0x0010;
	}

	inline std::string GetTypeFlags(int f)
	{
		std::string flags = "";
		if (f & 0x0001) flags += "Private | ";
		if (f & 0x0002) flags += "Famandassem | ";
		if (f & 0x0003) flags += "Assembly | ";
		if (f & 0x0004) flags += "Family | ";
		if (f & 0x0005) flags += "Famorassem | ";
		if (f & 0x0006) flags += "Public | ";
		if (f & 0x0010) flags += "Static | ";
		if (f & 0x0020) flags += "Initonly | ";
		if (f & 0x0040) flags += "Const | ";
		if (f & 0x0080) flags += "Notserialized | ";
		if (f & 0x0200) flags += "Specialname | ";
		if (f & 0x2000) flags += "Pinvokeimpl | ";
		if (flags.length() > 0)
		{
			flags.pop_back();
			flags.pop_back();
		}
		return flags;
	}

	inline void AddToI2HrameClasses(Il2CppClass* klass)
	{
		if (I2Hrame->m_mClasses.find(klass->image->assembly->aname.name) != I2Hrame->m_mClasses.end() && I2Hrame->m_mClasses[klass->image->assembly->aname.name].find(klass->namespaze) != I2Hrame->m_mClasses[klass->image->assembly->aname.name].end() && I2Hrame->m_mClasses[klass->image->assembly->aname.name][klass->namespaze].find(klass->name) != I2Hrame->m_mClasses[klass->image->assembly->aname.name][klass->namespaze].end())
			return;
		std::string assembly, nameSpace, name;
		std::string classSignature = Signature::Class::Create(klass);
		Signature::Class::Analysis(classSignature, &assembly, &nameSpace, &name);
		if (nameSpace.compare("") == 0) nameSpace = "__NO_NAMESPACE__";
		if (I2Hrame->m_mClasses.find(assembly) == I2Hrame->m_mClasses.end())
		{
			I2Hrame->m_mClasses.insert(std::make_pair(assembly, std::unordered_map<std::string, std::unordered_map<std::string, Il2CppClass*>>()));
		}
		if (I2Hrame->m_mClasses[assembly].find(nameSpace) == I2Hrame->m_mClasses[assembly].end())
		{
			I2Hrame->m_mClasses[assembly].insert(std::make_pair(nameSpace, std::unordered_map<std::string, Il2CppClass*>()));
		}
		I2Hrame->m_mClasses[assembly][nameSpace].insert(std::make_pair(classSignature, klass));
	}

	inline void AddToI2HrameClasses(const Il2CppType* type)
	{
		AddToI2HrameClasses(GetClassFromType(type));
	}

	inline bool IsEnglishName(std::string n)
	{
		for (auto c : n)
		{
			if ((const char*)c > u8"u0300")
				return false;
		}
		return true;
	}

	inline std::string ReplaceNamespace(std::string n)
	{
		for (auto c : n)  if (c == '.') n.replace(n.find(c), 1, "::");
		return n;
	}

	inline std::string CSharpTypeToCppType(const Il2CppType* t)
	{
		Il2CppClass* klass = GetClassFromType(t);
		std::string typeName = il2cpp_type_get_name(t);
		std::string resultCppType = "";
		if (typeName.compare("System.Boolean") == 0) resultCppType = "bool";
		else if (typeName.compare("System.Byte") == 0) resultCppType = "uint8_t";
		else if (typeName.compare("System.SByte") == 0) resultCppType = "int8_t";
		else if (typeName.compare("System.Char") == 0) resultCppType = "uint16_t";
		else if (typeName.compare("System.Int16") == 0) resultCppType = "int16_t";
		else if (typeName.compare("System.UInt16") == 0) resultCppType = "uint16_t";
		else if (typeName.compare("System.Int32") == 0) resultCppType = "int32_t";
		else if (typeName.compare("System.UInt32") == 0) resultCppType = "uint32_t";
		else if (typeName.compare("System.Int64") == 0) resultCppType = "int64_t";
		else if (typeName.compare("System.UInt64") == 0) resultCppType = "uint64_t";
		else if (typeName.compare("System.Single") == 0) resultCppType = "float";
		else if (typeName.compare("System.Double") == 0) resultCppType = "double";
		else if (typeName.compare("System.String") == 0) resultCppType = "Il2CppString*";
		else if (typeName.compare("System.Object") == 0) resultCppType = "Il2CppObject*";
		else if (typeName.compare("System.Void") == 0) resultCppType = "void";
		else if (klass->byval_arg.type == IL2CPP_TYPE_CLASS)
		{
			resultCppType = typeName + "*";
		}
		else if (klass->byval_arg.type == IL2CPP_TYPE_SZARRAY)
		{
			// 这个数组可能是一维数组也可能是二维数组甚至更多
			int rank = 0;
			// 根据klass->name，来判断他是多少维数组
			std::string arrayName = klass->name;
			// 用倒序来遍历更快
			for (int i = arrayName.length() - 1; i >= 0; i--)
			{
				if (arrayName[i] == '[')
					rank++;
				else if (arrayName[i] != ']')
					break;
			}
			// 获取到没有 '[]' 的类名
			arrayName = arrayName.substr(0, arrayName.length() - rank * 2);
			// 如果arrayName是个泛类, [T, TKey, TValue, TValueStorage, PromisedT]
			if (arrayName.compare("T") == 0 || arrayName.compare("TKey") == 0 || arrayName.compare("TValue") == 0 || arrayName.compare("TValueStorage") == 0 || arrayName.compare("PromisedT") == 0 || arrayName.compare("UnicodeChar") == 0)
			{
				resultCppType = "void*";
			}
			else
			{
				// 获取到数组的类型
				Il2CppClass* arrayBaseClass = I2Hrame->GetClassEx(klass->image->assembly->aname.name, klass->namespaze, arrayName);
				if (arrayBaseClass)
				{
					const Il2CppType* arrayBaseType = &arrayBaseClass->byval_arg;
					if (arrayBaseType->type == IL2CPP_TYPE_CLASS)
						resultCppType = std::string(il2cpp_type_get_name(arrayBaseType)) + "*";
				}
			}
			for (size_t i = 0; i < rank; i++)  resultCppType = "Array<" + resultCppType + ">*";
		}
		return ReplaceNamespace(resultCppType);
	}

	std::string ReplaceIllegalCharacters(std::string name)
	{
		for (auto c : name)
		{
			if (c == '<' || c == '>' || c == ':' || c == ' ' || c == ',' || c == '.' || c == '[' || c == ']' || c == '(' || c == ')' || c == '*' || c == '&' || c == '<' || c == '>' || c == '/' || c == '\\')
				name.replace(name.find(c), 1, "_");
		}
		return name;
	}

	class Console
	{
	public:
		std::string m_str;

		void Write(std::string str, ...)
		{
			va_list args;
			va_start(args, str);
			char buf[1024];
			vsprintf(buf, str.c_str(), args);
			va_end(args);
			m_str += buf;
		}

		void WriteLine(std::string str, ...)
		{
			va_list args;
			va_start(args, str);
			char buf[1024];
			vsprintf(buf, str.c_str(), args);
			va_end(args);
			m_str += buf;
			m_str += "\n";
		}

		void WriteLine()
		{
			m_str += "\n";
		}

		void Clear()
		{
			m_str.clear();
		}

		void Draw(ImVec2 size = ImVec2())
		{
			ImGui::InputTextMultiline("##console", &m_str, size, ImGuiInputTextFlags_ReadOnly);
		}
	};
}