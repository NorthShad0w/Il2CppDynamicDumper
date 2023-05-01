#pragma once

namespace PacketSniffer
{
	namespace Dumper
	{
		class DumperString
		{
		private:
			std::string m_str;
		public:
			DumperString() {}

			std::string GetContext() { return m_str; }

			void Write(std::string str, ...)
			{
				char buffer[1024];
				va_list args;
				va_start(args, str);
				vsprintf_s(buffer, str.c_str(), args);
				va_end(args);
				m_str += buffer;
			}

			void WriteLine(std::string str = "", ...)
			{
				char buffer[1024];
				va_list args;
				va_start(args, str);
				vsprintf_s(buffer, str.c_str(), args);
				va_end(args);
				m_str += buffer;
				m_str += "\n";
			}

			void WriteMethod(const MethodInfo* method, const char* parentName, bool bStatic)
			{
				if (bStatic)
					Write("\tstatic %s %s(", CSharpTypeToCppType(method->return_type).c_str(), method->name);
				else
					Write("\t%s %s(", CSharpTypeToCppType(method->return_type).c_str(), method->name);
				for (int i = 0; i < method->parameters_count; i++)
				{
					auto param = method->parameters[i];
					Write("%s %s", CSharpTypeToCppType(param.parameter_type).c_str(), param.name);
					if (i != method->parameters_count - 1) Write(", ");
				}
				WriteLine(")");
				WriteLine("\t{");
				if (bStatic)
					Write("\t\tstatic auto __fn = (%s(*)(", CSharpTypeToCppType(method->return_type).c_str());
				else
					Write("\t\tstatic auto __fn = (%s(*)(%s*, ", CSharpTypeToCppType(method->return_type).c_str(), parentName);
				for (int i = 0; i < method->parameters_count; i++)
				{
					auto param = method->parameters[i];
					Write("%s, ", CSharpTypeToCppType(param.parameter_type).c_str());
				}
				Write("MethodInfo*)(");
				WriteLine("I2Hrame->GetMethod(ThisClass(), \"%s\"));", Signature::Method::Create(method).c_str());
				Write("\t\t");
				if (method->return_type->type == IL2CPP_TYPE_VOID) Write("__fn(");
				else Write("return __fn(");
				if (!bStatic) Write("this, ");
				for (int i = 0; i < method->parameters_count; i++)
				{
					auto param = method->parameters[i];
					Write("%s", param.name);
					if (i != method->parameters_count - 1) Write(", ");
				}
				WriteLine(", nullptr);");
				WriteLine("\t}");
			}
		};

		class Single
		{
		private:
			inline static std::unordered_map<std::string, Single> m_map;
			Il2CppClass* m_pClassInstance = nullptr;
			std::string m_strClassSignature;
			std::vector<FieldInfo> m_vecInstanceFields;
			std::vector<FieldInfo> m_vecStaticFields;
			std::vector<const MethodInfo*> m_vecInstanceMethods;
			std::vector<const MethodInfo*> m_vecStaticMethods;
			std::vector<const MethodInfo*> m_vecConstructors;
			std::vector<const MethodInfo*> m_vecOperators;
			const MethodInfo* m_pToString = nullptr;
		public:
			Single() : m_pClassInstance(nullptr) {}
			Single(Il2CppClass* pClassInstance)
			{
				m_pClassInstance = pClassInstance;
				if (m_pClassInstance)
				{
					m_strClassSignature = Signature::Class::Create(pClassInstance);
					if (m_map.find(m_strClassSignature) == m_map.end())
						m_map[m_strClassSignature] = *this;
					else
						*this = m_map[m_strClassSignature];

					if (m_pClassInstance->field_count > 0)
					{
						for (size_t i = 0; i < m_pClassInstance->field_count; i++)
						{
							auto field = m_pClassInstance->fields[i];
							if ((field.type->attrs & 0x0010) == 0) m_vecInstanceFields.push_back(field);
							else m_vecStaticFields.push_back(field);
						}
					}
					if (m_pClassInstance->method_count > 0)
					{
						for (size_t i = 0; i < m_pClassInstance->method_count; i++)
						{
							auto method = m_pClassInstance->methods[i];
							if (!method) continue;
							if (method->name[0] == '.' && method->name[1] == 'c') continue;
							else if (method->name[0] == 'o' && method->name[1] == 'p' && method->name[2] == '_') m_vecOperators.push_back(method);
							else if (method->name[0] == 'T' && method->name[1] == 'o' && method->name[2] == 'S' && method->name[3] == 't' && method->name[4] == 'r' && method->name[5] == 'i' && method->name[6] == 'n' && method->name[7] == 'g') m_pToString = method;
							else if (IsMethodStatic(method)) m_vecStaticMethods.push_back(method);
							else if (!IsMethodStatic(method)) m_vecInstanceMethods.push_back(method);
						}
					}
				}
			}

			std::string DumpClass(bool bStaticMember, bool bInstanceMember, bool bStaticMethod, bool bInstanceMethod, bool bToStirng, bool bOperator, bool bDefine, bool bInheritance, bool bFlags)
			{
				DumperString dumper = DumperString();
				if (bDefine)
				{
					dumper.WriteLine("#define CLASS(assembly, namespaze, klass) static Il2CppClass* ThisClass() { return I2Hrame->GetClassEx(assembly, namespaze, klass); }");
					dumper.WriteLine("#define MEMBER(klass, name, offset)  struct { char __pad__##name[offset]; klass name; }");
					dumper.WriteLine("#if _WIN64");
					dumper.WriteLine("#define STATIC_MEMBER(klass, name, offset) static klass get_##name() { return *reinterpret_cast<klass*>(*reinterpret_cast<uintptr_t*>((uintptr_t)ThisClass() + 0xB8) + offset); } static void set_##name(klass value) { *reinterpret_cast<klass*>(*reinterpret_cast<uintptr_t*>((uintptr_t)ThisClass() + 0xB8) + offset) = value; }");
					dumper.WriteLine("#elif");
					dumper.WriteLine("#define  STATIC_MEMBER(klass, name, offset) static klass get_##name() { return *reinterpret_cast<klass*>(*reinterpret_cast<uintptr_t*>((uintptr_t)ThisClass() + 0x5C) + offset); } static void set_##name(klass value) { *reinterpret_cast<klass*>(*reinterpret_cast<uintptr_t*>((uintptr_t)ThisClass() + 0x5C) + offset) = value; }");
					dumper.WriteLine("#endif");
					dumper.WriteLine();
					dumper.WriteLine();
					dumper.WriteLine();
				}
				if (bInheritance)
				{
					dumper.Write("// Inheritance: ");
					auto klass = m_pClassInstance;
					while (klass)
					{
						dumper.Write(klass->name);
						if (klass->parent)
							dumper.Write(" -> ");
						klass = klass->parent;
					}
					dumper.WriteLine();
				}
				if (bFlags)
				{
					dumper.Write("// Flags: ");
					dumper.WriteLine(GetTypeFlags(m_pClassInstance->flags));
				}
				dumper.Write("class %s", m_pClassInstance->name);
				if (m_pClassInstance->parent)
				{
					dumper.Write(" : public ");
					if (strlen(m_pClassInstance->parent->namespaze) > 0)
					{
						for (auto c : std::string(m_pClassInstance->parent->namespaze))
						{
							if (c == '.')
								dumper.Write("::");
							else
								dumper.Write("%c", c);
						}
						dumper.Write("::");
					}
					dumper.WriteLine(m_pClassInstance->parent->name);
				}
				else dumper.WriteLine();
				dumper.WriteLine("{");
				dumper.WriteLine("public:");
				dumper.Write("\tCLASS(\"%s\", \"%s\", ", m_pClassInstance->image->assembly->aname.name, m_pClassInstance->namespaze);
				if (IsEnglishName(m_pClassInstance->name))
					dumper.WriteLine("\"%s\");", m_pClassInstance->name);
				else
					dumper.WriteLine("u8\"%s\");", m_pClassInstance->name);
				dumper.WriteLine();
				if (bStaticMember && m_vecStaticFields.size() > 0)
				{
					for (auto& field : m_vecStaticFields)
					{
						dumper.WriteLine("\tSTATIC_MEMBER(%s, %s, %d);", CSharpTypeToCppType(field.type).c_str(), field.name, field.offset);
					}
					dumper.WriteLine();
				}
				if (bInstanceMember && m_vecInstanceFields.size() > 0)
				{
					dumper.WriteLine("\tunion"); dumper.WriteLine("\t{");
					for (auto& field : m_vecInstanceFields)
					{
						dumper.WriteLine("\t\tMEMBER(%s, %s, %d);", CSharpTypeToCppType(field.type).c_str(), field.name, field.offset);
					}
					dumper.WriteLine("\t};");
					dumper.WriteLine();
				}
				if (bStaticMethod && m_vecStaticMethods.size() > 0)
				{
					for (auto& method : m_vecStaticMethods)
					{
						dumper.WriteMethod(method, m_pClassInstance->name, true);
					}
					dumper.WriteLine();
				}
				if (bInstanceMethod && m_vecInstanceMethods.size() > 0)
				{
					for (auto& method : m_vecInstanceMethods)
					{
						dumper.WriteMethod(method, m_pClassInstance->name, false);
					}
				}
				if (bToStirng && m_pToString)
				{
					dumper.WriteMethod(m_pToString, m_pClassInstance->name, false);
				}
				if (bOperator && m_vecOperators.size() > 0)
				{
					for (auto& method : m_vecOperators)
					{
						dumper.WriteMethod(method, m_pClassInstance->name, false);
					}
				}
				dumper.WriteLine("};");
				return dumper.GetContext();
			}

			std::string DumpEnum()
			{
				DumperString dumper = DumperString();
				dumper.WriteLine("enum class %s", m_pClassInstance->name);
				dumper.WriteLine("{");
				for (size_t i = 0; i < m_vecStaticFields.size(); i++)
				{
					auto field = m_vecStaticFields[i];
					dumper.Write("\t%s = %d", field.name, i);
					if (i != m_vecInstanceFields.size() - 1)
						dumper.WriteLine(",");
				}
				dumper.WriteLine("};");
				return dumper.GetContext();
			}

			std::string DumpStruct()
			{
				DumperString dumper = DumperString();
				dumper.WriteLine("struct %s", m_pClassInstance->name);
				dumper.WriteLine("{");
				for (auto& field : m_vecInstanceFields)
				{
					dumper.WriteLine("\t%s %s;", CSharpTypeToCppType(field.type).c_str(), field.name);
				}
				dumper.WriteLine("};");
				return dumper.GetContext();
			}
		};
	}

	inline void FieldInfoElement(FieldInfo field, Il2CppClass** selectedClass)
	{
		std::string fieldName = field.name;
		std::string typeName = il2cpp_type_get_name(field.type);
		ImGui::AlignTextToFramePadding();
		if (field.type->attrs & 0x0040)
			ImGui::Text("%s ( Type: %s ) (const)", fieldName.c_str(), typeName.c_str());
		ImGui::Text("%s %s;// Offset: 0x%04X", typeName.c_str(), fieldName.c_str(), field.offset);
		ImGui::SameLine();
		if (ImGui::Button(("JumpType##" + fieldName).c_str()))
		{
			*selectedClass = GetClassFromType(field.type);
		}
	}

	inline void MethodInfoElement(const MethodInfo* method, Il2CppClass** selectedClass)
	{
		std::string methodName = method->name;
		std::string methodFlags = GetMethodFlags(method->flags);
		std::string methodReturnType = il2cpp_type_get_name(method->return_type);
		std::string methodParameters = "";
		std::vector<std::string> methodParametersVector;
		for (int i = 0; i < method->parameters_count; i++)
		{
			methodParameters += il2cpp_type_get_name(method->parameters[i].parameter_type);
			if (i != method->parameters_count - 1)
				methodParameters += ", ";
			methodParametersVector.push_back(il2cpp_type_get_name(method->parameters[i].parameter_type));
		}
		ImGui::AlignTextToFramePadding();
		ImGui::Text("%s %s(%s)", methodReturnType.c_str(), methodName.c_str(), methodParameters.c_str());
		ImGui::Separator();
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Method Name: %s", methodName.c_str());;
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Method Flags: %s", methodFlags.c_str());
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Method Return Type: %s", methodReturnType.c_str());
		ImGui::SameLine();
		if (ImGui::Button(("Jump##" + methodReturnType).c_str()))
			*selectedClass = GetClassFromType(method->return_type);
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Method Parameters:");
		for (size_t i = 0; i < methodParametersVector.size(); i++)
		{
			ImGui::SameLine();
			auto parameter = methodParametersVector[i];
			if (ImGui::Button((parameter + "##" + methodReturnType + std::to_string(i)).c_str()))
				*selectedClass = GetClassFromType(method->parameters[i].parameter_type);
			if (i != methodParametersVector.size() - 1)
			{
				ImGui::SameLine();
				ImGui::AlignTextToFramePadding();
				ImGui::Text(",");
			}
		}
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Method Parameters Count: %d", method->parameters_count);
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Method Slot: %d", method->slot);
	}

	inline void Update()
	{
		if (!GlobalSetting::ShowMenu) return;
		static ImGuiWindowFlags classFinderWindowFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
		ImGui::Begin("PacketSniffer", 0, classFinderWindowFlags);
		static bool pinWindow = true;
		static std::string message = "[!] Uninitialized now. Choose the assembly first.(PS: Double click the assembly name to choose it)";
		if (ImGui::Checkbox("Pin Window", &pinWindow))
		{
			if (pinWindow)
			{
				classFinderWindowFlags |= ImGuiWindowFlags_NoMove;
				classFinderWindowFlags |= ImGuiWindowFlags_NoResize;
			}
			else
			{
				classFinderWindowFlags &= ~ImGuiWindowFlags_NoMove;
				classFinderWindowFlags &= ~ImGuiWindowFlags_NoResize;
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Reset Position")) ImGui::SetWindowPos(ImVec2(0, 0));
		ImGui::SameLine();
		if (ImGui::Button("Reset Size")) ImGui::SetWindowSize(ImGui::GetIO().DisplaySize);
		if (GlobalSetting::Init)
		{
			ImGui::SameLine();
			if (ImGui::Button("Rest PackSniffer"))
			{
				GlobalSetting::Init = false;
				I2Hrame->m_mClasses = std::unordered_map<std::string, std::unordered_map<std::string, std::unordered_map<std::string, Il2CppClass*>>>();
				if (I2Hrame->m_mClasses.size() <= 0)
					message = "[+] Reset PackSniffer Successful";
			}
		}
		ImGui::SameLine();
		ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
		ImGui::SameLine();
		ImGui::Text("Last Message:"); ImGui::SameLine();
		if (message[1] == '+') ImGui::TextColored(ImVec4(0, 1, 0, 1), message.c_str());
		else if (message[1] == '-') ImGui::TextColored(ImVec4(1, 0, 0, 1), message.c_str());
		else if (message[1] == '!') ImGui::TextColored(ImVec4(1, 1, 0, 1), message.c_str());
		else ImGui::Text(message.c_str());

		ImGui::BeginChild("##OperatingArea");
		if (GlobalSetting::Init)
		{
			do
			{
				static Il2CppClass* selectedClass = nullptr;
				static bool filtrationNotEnglishName = false;
				static bool onlyNotEnglishName = false;
				ImGui::BeginChild("##ClassList", ImVec2(ImGui::GetWindowWidth() * 0.3f, 0), true);
				{
					static ImGuiTextFilter filterModule = "Assembly-CSharp";
					static ImGuiTextFilter filterNamespace = "__NO_NAMESPACE__";
					static ImGuiTextFilter filterClass;
					if (ImGui::Checkbox(u8"Filter Not English Name Item", &filtrationNotEnglishName))
						onlyNotEnglishName = false;
					ImGui::SameLine();
					if (ImGui::Checkbox(u8"Only Not English Name Item", &onlyNotEnglishName))
						filtrationNotEnglishName = false;
					filterModule.Draw(u8"Filter Module");
					filterNamespace.Draw(u8"Filter Namespace");
					filterClass.Draw(u8"Filter Class");
					ImGui::Separator();
					for (auto& [moduleName, namespaces] : I2Hrame->m_mClasses)
					{
						if (!filterModule.PassFilter(moduleName.c_str())) continue;
						if (ImGui::TreeNodeEx(moduleName.c_str(), ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen))
						{
							for (auto& [namespaceName, classese] : namespaces)
							{
								if (!filterNamespace.PassFilter(namespaceName.c_str())) continue;
								if (ImGui::TreeNodeEx(namespaceName.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
								{
									for (auto& [classname, klass] : classese)
									{
										std::string assembly, nameSpace, name;
										Signature::Class::Analysis(ConfusedTranslate::ConvertKlass(classname), &assembly, &nameSpace, &name);
										if (filtrationNotEnglishName && !IsEnglishName(name)) continue;
										if (onlyNotEnglishName && IsEnglishName(name)) continue;
										if (klass->enumtype)
										{
											name = "(Enum) " + name;
										}
										else
										{
											switch (klass->byval_arg.type)
											{
											case IL2CPP_TYPE_CLASS:
												name = "(Class) " + name;
												break;
											case IL2CPP_TYPE_VALUETYPE:
												name = "(Struct) " + name;
												break;
											default:
												break;
											}
										}
										if (filterClass.PassFilter(name.c_str()))
										{
											ImGui::AlignTextToFramePadding();
											if (ImGui::Selectable(name.c_str(), selectedClass == klass, 0, ImVec2(ImGui::GetWindowWidth() * 0.5f, 0.f)))
											{
												selectedClass = klass;
											}
											if (klass->parent)
											{
												ImGui::SameLine();
												if (ImGui::Button(("Parent##" + std::string(name)).c_str()))
												{
													selectedClass = klass->parent;
												}
											}
										}
									}
									ImGui::TreePop();
								}
							}
							ImGui::TreePop();
						}
					}
				}
				ImGui::EndChild();

				ImGui::SameLine();

				ImGui::BeginChild("##ClassInfo", ImVec2(ImGui::GetWindowWidth() * 0.4f, 0), true);
				do
				{
					if (selectedClass == nullptr) break;
					std::string classSignature = Signature::Class::Create(selectedClass);
					ImGui::Text("[ %s ] Name: %s", classSignature.c_str(), selectedClass->name);
					ImGui::Separator();
					ImGui::AlignTextToFramePadding();
					ImGui::Text("Inheritance:");
					if (selectedClass->parent)
					{
						ImGui::SameLine();
						Il2CppClass* parent = selectedClass->parent;
						int index = 0;
						while (parent)
						{
							if (ImGui::Button((parent->name + std::string("##") + std::to_string(index)).c_str()))
								selectedClass = parent;
							parent = parent->parent;
							if (parent)
							{
								ImGui::SameLine();
								ImGui::AlignTextToFramePadding();
								ImGui::Text("<-");
								ImGui::SameLine();
							}
							index++;
						}
					}
					ImGui::Text("Namespace: %s", selectedClass->namespaze);
					ImGui::Text("Assembly: %s", selectedClass->image->assembly->aname.name);
					ImGui::Text("Flags: %s", GetTypeFlags(selectedClass->flags).c_str());
					ImGui::Separator();
					if (!((bool)selectedClass->enumtype))
					{
						static bool showMethods = false;
						{
							static ImGuiTextFilter filter;
							static const MethodInfo* selectedMethod = nullptr;
							ImGui::AlignTextToFramePadding();
							ImGui::Text("Methods: ");
							ImGui::SameLine();
							filter.Draw();
							ImGui::BeginChild(u8"##Methods", ImVec2(0, ImGui::GetWindowHeight() * 0.5f), true);
							{
								if (ImGui::BeginListBox("##MethodList", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.35f, ImGui::GetWindowContentRegionMax().y - ImGui::GetWindowContentRegionMin().y)))
								{
									for (int i = 0; i < selectedClass->method_count; i++)
									{
										auto method = selectedClass->methods[i];
										if (!method) continue;
										std::string name = il2cpp_type_get_name(method->return_type); name += " "; name += method->name; name += "("; for (uint32_t j = 0; j < method->parameters_count; j++) { name += il2cpp_type_get_name(method->parameters[j].parameter_type); if (j != method->parameters_count - 1) name += ", "; } name += ")";
										if (!filter.PassFilter(name.c_str())) continue;
										if (ImGui::Selectable(name.c_str(), selectedMethod == method, 0, ImVec2(0.f, 0.f)))
										{
											selectedMethod = method;
										}
									}
									ImGui::EndListBox();
								}
								ImGui::SameLine();
								ImGui::BeginChild(u8"##MethodInfo", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
								{
									if (selectedMethod && selectedMethod->klass == selectedClass)
										MethodInfoElement(selectedMethod, &selectedClass);
									else
										ImGui::Text("Select a method to view info");
								}
								ImGui::EndChild();
							}
							ImGui::EndChild();
						}
						ImGui::Separator();
						{
							static ImGuiTextFilter filter;
							ImGui::AlignTextToFramePadding();
							ImGui::Text("Fields: ");
							ImGui::SameLine();
							filter.Draw();
							ImGui::BeginChild("##Static", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 0.f), true, ImGuiWindowFlags_HorizontalScrollbar);
							{
								ImGui::Text("Static");
								ImGui::Separator();
								for (int i = 0; i < selectedClass->field_count; i++)
								{
									auto field = selectedClass->fields[i];
									if ((field.type->attrs & 0x0010) == 0) continue;
									if (!filter.PassFilter(field.name)) continue;
									FieldInfoElement(field, &selectedClass);
								}
							}
							ImGui::EndChild();

							ImGui::SameLine();

							ImGui::BeginChild("##Instance", ImVec2(0.f, 0.f), true, ImGuiWindowFlags_HorizontalScrollbar);
							{
								ImGui::Text("Instance");
								ImGui::Separator();
								for (int i = 0; i < selectedClass->field_count; i++)
								{
									auto field = selectedClass->fields[i];
									if ((field.type->attrs & 0x0010) != 0) continue;
									if (!filter.PassFilter(field.name)) continue;
									FieldInfoElement(field, &selectedClass);
								}
							}
							ImGui::EndChild();
						}
					}
					else
					{
						for (int i = 0; i < selectedClass->field_count; i++)
						{
							auto field = selectedClass->fields[i];
							if ((field.type->attrs & 0x0010) == 0) continue;
							std::string fieldName = field.name;
							ImGui::Text("%s : %d", fieldName.c_str(), i);
						}
					}
				} while (false);
				ImGui::EndChild();

				ImGui::SameLine();

				ImGui::BeginChild("##ClassStruct", ImVec2(0, 0), true);
				do
				{
					// 两个tab页
					if (ImGui::BeginTabBar("##ClassStructTabBar"))
					{
						if (ImGui::BeginTabItem("Single"))
						{
							if (selectedClass)
							{
								static std::string classStruct = "";
								static Dumper::Single dump = Dumper::Single(selectedClass);
								if (selectedClass->enumtype)
								{
									if (ImGui::Button("Dump Cpp Enum", ImVec2(-FLT_MIN, 30.f)))
									{
										dump = Dumper::Single(selectedClass);
										classStruct = dump.DumpEnum();
									}
								}
								else
								{
									switch (selectedClass->byval_arg.type)
									{
									case IL2CPP_TYPE_CLASS:
									{
										static bool bInstanceMember = true;
										static bool bStaticMember = true;
										static bool bInstanceMethod = true;
										static bool bStaticMethod = true;
										static bool bInhertitance = true;
										static bool bToString = true;
										static bool bOperators = true;
										static bool bFlags = true;
										static bool bDefine = true;
										ImGui::Checkbox("Instance Member", &bInstanceMember); ImGui::SameLine();
										ImGui::Checkbox("Static Member", &bStaticMember); ImGui::SameLine();
										ImGui::Checkbox("Instance Method", &bInstanceMethod); ImGui::SameLine();
										ImGui::Checkbox("Static Method", &bStaticMethod);
										ImGui::Checkbox("Inhertitance", &bInhertitance); ImGui::SameLine();
										ImGui::Checkbox("Flags", &bFlags); ImGui::SameLine();
										ImGui::Checkbox("ToString", &bToString); ImGui::SameLine();
										ImGui::Checkbox("Operators", &bOperators); ImGui::SameLine();
										ImGui::Checkbox("Define", &bDefine);
										if (ImGui::Button("Dump Cpp Class", ImVec2(-FLT_MIN, 30.f)))
										{
											dump = Dumper::Single(selectedClass);
											classStruct = dump.DumpClass(bStaticMember, bInstanceMember, bStaticMethod, bInstanceMethod, bToString, bOperators, bDefine, bInhertitance, bFlags);
										}
									}
									break;
									case IL2CPP_TYPE_VALUETYPE:
									{
										if (ImGui::Button("Dump Cpp Struct", ImVec2(-FLT_MIN, 30.f)))
										{
											dump = Dumper::Single(selectedClass);
											classStruct = dump.DumpStruct();
										}
									}
									break;
									default:
									{
										ImGui::Text("Not supported");
									}
									break;
									}

								}
								ImGui::Separator();
								ImGui::InputTextMultiline("##ClassStruct", &classStruct, ImVec2(-FLT_MIN, -FLT_MIN), ImGuiInputTextFlags_ReadOnly);
							}
							else
							{
								ImGui::Text("Select a class to view info");
							}
							ImGui::EndTabItem();
						}
						if (ImGui::BeginTabItem("Multiple"))
						{
							static std::string outPath = "";
							static std::string outName = "";
							ImGui::Text("PS: In This Dumper Mode Will Dump All Readied Classes");
							ImGui::Separator();
							ImGui::InputText("Output Path", &outPath);
							ImGui::InputText("Output Name", &outName);
							ImGui::Separator();
							ImGui::Text("Dump Class Settings: ");
							static bool bInstanceMember = true;
							static bool bStaticMember = true;
							static bool bInstanceMethod = true;
							static bool bStaticMethod = true;
							static bool bInhertitance = true;
							static bool bToString = true;
							static bool bOperators = true;
							static bool bFlags = true;
							static bool bDefine = true;
							ImGui::Checkbox("Instance Member", &bInstanceMember); ImGui::SameLine();
							ImGui::Checkbox("Static Member", &bStaticMember); ImGui::SameLine();
							ImGui::Checkbox("Instance Method", &bInstanceMethod); ImGui::SameLine();
							ImGui::Checkbox("Static Method", &bStaticMethod);
							ImGui::Checkbox("Inhertitance", &bInhertitance); ImGui::SameLine();
							ImGui::Checkbox("Flags", &bFlags); ImGui::SameLine();
							ImGui::Checkbox("ToString", &bToString); ImGui::SameLine();
							ImGui::Checkbox("Operators", &bOperators); ImGui::SameLine();
							ImGui::Checkbox("Define", &bDefine);
							ImGui::Separator();
							static std::vector<std::string> namespaceFilter;
							static ImGuiTextFilter filter;
							ImGui::Text("Select the namespace where you do not want to dump.");
							filter.Draw();
							if (ImGui::BeginListBox("##NamespaceFilter", ImVec2(-FLT_MIN, 200.f)))
							{
								for (auto& [moduleName, namespaces] : I2Hrame->m_mClasses)
								{
									for (auto& [namespaceName, classese] : namespaces)
									{
										if (!filter.PassFilter(namespaceName.c_str())) continue;
										if (ImGui::Selectable(namespaceName.c_str(), std::find(namespaceFilter.begin(), namespaceFilter.end(), namespaceName) != namespaceFilter.end()))
										{
											if (std::find(namespaceFilter.begin(), namespaceFilter.end(), namespaceName) != namespaceFilter.end())
											{
												namespaceFilter.erase(std::find(namespaceFilter.begin(), namespaceFilter.end(), namespaceName));
											}
											else
											{
												namespaceFilter.push_back(namespaceName);
											}
										}
									}
								}
								ImGui::EndListBox();
							}
							static Console console;
							if (ImGui::Button("Dump All Readied Classes", ImVec2(-FLT_MIN, 30.f)))
							{
								if (!outPath.empty() && !outName.empty())
								{
									console.Clear();
									std::string classPath = outPath + "\\" + outName;
									if (!std::filesystem::exists(classPath)) std::filesystem::create_directories(classPath);
									for (auto& [moduleName, namespaces] : I2Hrame->m_mClasses)
									{
										classPath = outPath + "\\" + outName + "\\" + moduleName;
										if (!std::filesystem::exists(classPath)) std::filesystem::create_directories(classPath);
										for (auto& [namespaceName, classese] : namespaces)
										{
											if (std::find(namespaceFilter.begin(), namespaceFilter.end(), namespaceName) != namespaceFilter.end()) continue;
											classPath = outPath + "\\" + outName + "\\" + moduleName + "\\" + namespaceName;
											if (!std::filesystem::exists(classPath)) std::filesystem::create_directories(classPath);
											for (auto& [className, classInfo] : classese)
											{
												Dumper::Single classDumper = Dumper::Single(classInfo);
												std::string classStruct;
												if (classInfo->enumtype)
												{
													classStruct = classDumper.DumpEnum();
												}
												else
												{
													switch (classInfo->byval_arg.type)
													{
													case IL2CPP_TYPE_CLASS:
														classStruct = classDumper.DumpClass(bInstanceMember, bStaticMember, bInstanceMethod, bStaticMethod, bInhertitance, bToString, bOperators, bFlags, bDefine);
														break;
													case IL2CPP_TYPE_VALUETYPE:
														classStruct = classDumper.DumpStruct();
														break;
													default:
														break;
													}
												}
												if (!classStruct.empty())
												{
													std::ofstream ofs(classPath + "\\" + ReplaceIllegalCharacters(classInfo->name) + ".h");
													if (ofs.is_open())
													{
														ofs << classStruct;
														ofs.close();
													}
													else
													{
														console.WriteLine("[-] Failed to open file: " + classPath + "\\" + ReplaceIllegalCharacters(classInfo->name) + ".h");
													}
												}
											}
										}
									}
								}
								else
								{
									message = "[-] Output Path or Output Name is empty";
								}
							}
							console.Draw(ImVec2(-FLT_MIN, -FLT_MIN));
							ImGui::EndTabItem();
						}
						ImGui::EndTabBar();
					}

				} while (false);
				ImGui::EndChild();
			} while (false);
		}
		else
		{
			do
			{
				static std::vector<Il2CppAssembly*> readyAssemblies = {};

				ImGui::BeginChild(u8"##AssmeblyList", ImVec2(ImGui::GetWindowWidth() * 0.3f, 0), true);
				{
					static ImGuiTextFilter filterModule = "";
					filterModule.Draw(u8"Filter Module");
					size_t assemblyCount = 0;
					Il2CppAssembly** assemblies = GetAssembliesFromDomain(I2Hrame->m_pDomain, &assemblyCount);
					for (size_t i = 0; i < assemblyCount; i++)
					{
						auto assembly = assemblies[i];
						std::string assemblyName = assembly->aname.name;
						if (!filterModule.PassFilter(assemblyName.c_str())) continue;
						if (std::find(readyAssemblies.begin(), readyAssemblies.end(), assembly) != readyAssemblies.end()) continue;
						ImGui::AlignTextToFramePadding();
						if (ImGui::Selectable(assemblyName.c_str(), false, ImGuiSelectableFlags_AllowDoubleClick))
						{
							if (ImGui::IsMouseDoubleClicked(0))
							{
								readyAssemblies.push_back(assembly);
							}
						}
					}
				}
				ImGui::EndChild();

				ImGui::SameLine();

				ImGui::BeginChild(u8"##Assembly", ImVec2(0, 0), true);
				{
					// 顶部显示确定按钮，接着他的下面显示readyAssemblies
					if (ImGui::Button("Confirm & Init"))
					{
						for (auto assembly : readyAssemblies)
						{
							if (!assembly) continue;
							auto image = assembly->image;
							if (!image) continue;
							size_t classCount = GetClassCountFromImage(image);
							for (size_t j = 0; j < classCount; j++)
							{
								auto klass = GetClassFromImage(image, j);
								if (!klass) continue;
								AddToI2HrameClasses(klass);
							}
						}
						if (readyAssemblies.size() > 0)
						{
							message = "[+] Assemblies are ready.";
							GlobalSetting::Init = true;
						}
						else
						{
							message = "[-] No assemblies ready.";
						}
					}
					ImGui::SameLine();
					if (ImGui::Button("Clear"))
					{
						readyAssemblies.clear();
					}
					ImGui::Separator();
					ImGui::Text("Ready Assemblies:");
					for (auto assembly : readyAssemblies)
					{
						std::string assemblyName = assembly->aname.name;
						ImGui::Text(("\t-- " + assemblyName).c_str());
						ImGui::SameLine();
						if (ImGui::Button(("Unload##" + assemblyName).c_str()))
						{
							if (std::find(readyAssemblies.begin(), readyAssemblies.end(), assembly) != readyAssemblies.end())
							{
								readyAssemblies.erase(std::find(readyAssemblies.begin(), readyAssemblies.end(), assembly));
							}
							else
							{
								message = "[!] This assembly is not ready.";
							}
						}
					}
				}
				ImGui::EndChild();
			} while (false);
		}
		ImGui::EndChild();
		ImGui::End();
	}

	namespace Hooks
	{
		//namespace _PlayerNetwork
		//{
		//	//using GetMovementSpeedFn = float(*)(PlayerNetwork* __this, MethodInfo* method);
		//	//inline GetMovementSpeedFn oGetMovementSpeed = nullptr;
		//	//inline float GetMovementSpeed(PlayerNetwork* __this, MethodInfo* method)
		//	//{
		//	//	if (GlobalSetting::Unload || !config.fight.movement.move.binding.state || __this == nullptr || __this != PlayerNetwork::get_LocalPlayer())
		//	//		return oGetMovementSpeed(__this, method);
		//	//	return oGetMovementSpeed(__this, method) * config.fight.movement.move.fSpeedAddtion;
		//	//}
		//}

		inline void Setup()
		{
			static auto CreateHook = [](LPVOID pTarget, LPVOID pDetour, LPVOID* ppOriginal)->bool
			{
				return MH_CreateHook(pTarget, pDetour, ppOriginal) == MH_OK && MH_EnableHook(pTarget) == MH_OK;
			};
			//if (!CreateHook(
			//	I2Hrame->GetMethod(PlayerNetwork::ThisClass(), "System.Single GetMovementSpeed()"),
			//	Hooks::_PlayerNetwork::GetMovementSpeed,
			//	reinterpret_cast<void**>(&Hooks::_PlayerNetwork::oGetMovementSpeed)
			//))
			//	printf("[-] Failed to create hook for PlayerNetwork::GetMovementSpeed\n");
		}
	}

	inline void Setup()
	{
		char path[MAX_PATH];
		GetModuleFileNameA(NULL, path, MAX_PATH);
		std::filesystem::path rootPath = path;
		rootPath = rootPath.parent_path() / "confuser.json";
		std::ifstream file(rootPath);
		if (file.is_open())
		{
			nlohmann::json json;
			file >> json;
			file.close();

			if (!json["klass"].empty())
			{
				for (auto j : json["klass"])
				{
					ConfusedTranslate::Klass k;
					k.assembly = j["assembly"];
					k.nameSpace = j["nameSpace"];
					k.originalName = j["originalName"];
					k.confusedName = std::string(u8"" + j["confusedName"].get<std::string>());
					ConfusedTranslate::klass.push_back(k);
				}
			}

			if (!json["method"].empty())
			{
				for (auto j : json["method"])
				{
					ConfusedTranslate::Method m;
					m.klass = ConfusedTranslate::klass[j["klass"].get<int>()];
					m.originalName = j["originalName"];
					m.confusedName = std::string(u8"" + j["confusedName"].get<std::string>());
				}
			}
		}

		Hooks::Setup();

		auto hook = ImGuiContextHook();
		hook.Callback = (ImGuiContextHookCallback)Update;
		hook.Type = ImGuiContextHookType_NewFramePost;
		ImGui::AddContextHook(ImGui::GetCurrentContext(), &hook);
	}
}