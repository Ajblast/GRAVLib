function(setMSVCRuntime TargetName DynamicRuntime)
	#Should the lib be compiled with a dynamic msvc runtime or static
	if(DynamicRuntime)
		set_property(TARGET ${TargetName} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
	else()
		set_property(TARGET ${TargetName} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
	endif()
endfunction()

function(makelib LibName LibPath LibType DynamicRuntime pchName BuildTests)
	set(LibName ${LibName} PARENT_SCOPE)
	set(TargetName ${LibName})

	set(IncludePath ${LibPath}/include)
	set(SourcePath ${LibPath}/src)
	set(TestsPath ${LibPath}/tests)

	#Create the library
	add_library(${LibName} ${LibType})
	add_subdirectory(${IncludePath})
	add_subdirectory(${SourcePath})
	target_precompile_headers(${LibName} PRIVATE ${pchName})
	target_include_directories(${LibName} PUBLIC ${IncludePath})

	# Is the lib a static or dynamic lib
	if(${LibType} STREQUAL SHARED)
		target_compile_definitions(${LibName} PUBLIC "${LibName}_EXPORTS")
	elseif(${LibType} STREQUAL STATIC)
	endif()

	#Should the exe be compiled with a dynamic msvc runtime or static
	setmsvcruntime(${TargetName} ${DynamicRuntime})

	#Print debug info about the project
	if(DebugInfo)
		message("")
		message("Compile Library: ${LibName}")
		message("Library Type: ${LibType}")
		message("Dynamic Runtime: ${DynamicRuntime}")

		message("Project Language: ${ProjectLanguage}")
		if (${ProjectLanguage} STREQUAL CXX)
			message("C Standard: ${CMAKE_C_STANDARD}")
			message("CXX Standard: ${CMAKE_CXX_STANDARD}")
		endif()

		message("")
		message("Include Path: ${IncludePath}")
		message("Source Path: ${SourcePath}")
		message("Tests Path: ${TestsPath}")

		if(BuildTypes)
			message("")
			message("Build Type Compiler Flags")
			foreach(BuildType ${BuildTypes})
				message("${BuildType} Flags: ${CMAKE_${ProjectLanguage}_FLAGS_${BuildType}}")
			endforeach()
		endif()
	endif()

	#Build the tests if they should be created
	if(BuildTests)
		add_subdirectory(${TestsPath})
	endif()
endfunction()

function(makeexe ExeName ExePath DynamicRuntime pchName BuildTests)
	set(ExeName ${ExeName} PARENT_SCOPE)
	set(TargetName ${LibName})

	set(IncludePath ${ExePath}/include)
	set(SourcePath ${ExePath}/src)
	set(TestsPath ${ExePath}/tests)

	#Create the Exerary
	add_executable(${ExeName})
	add_subdirectory(${IncludePath})
	add_subdirectory(${SourcePath})
	target_precompile_headers(${ExeName} PRIVATE ${pchName})
	target_include_directories(${ExeName} PUBLIC ${IncludePath})

	#Should the exe be compiled with a dynamic msvc runtime or static
	setmsvcruntime(${TargetName} ${DynamicRuntime})

	#Print debug info about the project
	if(DebugInfo)
		message("")
		message("Compile Executable: ${ExeName}")
		message("Dynamic Runtime: ${DynamicRuntime}")
		message("Project Language: ${ProjectLanguage}")
		if (${ProjectLanguage} STREQUAL CXX)
			message("C Standard: ${CMAKE_C_STANDARD}")
			message("CXX Standard: ${CMAKE_CXX_STANDARD}")
		endif()

		message("")
		message("Include Path: ${IncludePath}")
		message("Source Path: ${SourcePath}")
		message("Tests Path: ${TestsPath}")

		if(BuildTypes)
			message("")
			message("Build Type Compiler Flags")
			foreach(BuildType ${BuildTypes})
				message("${BuildType} Flags: ${CMAKE_${ProjectLanguage}_FLAGS_${BuildType}}")
			endforeach()
		endif()
	endif()

	#Build the tests if they should be created
	if(BuildTests)
		add_subdirectory(${TestsPath})
	endif()
endfunction()

function(maketest TargetName DynamicRuntime SourceFiles LinkLibraries)
	if(DebugInfo)
		message("")
		message("Make Tests: ${TargetName}")
		message("Dynamic Runtime: ${DynamicRuntime}")
		message("Source Files: ${SourceFiles}")
		message("Link Libraries: ${LinkLibraries}")
	endif()

	add_executable(${TargetName} ${SourceFiles})
	target_link_libraries(${TargetName} GTest::gtest_main ${LinkLibraries})
	
	setmsvcruntime(${TargetName} ${DynamicRuntime})

	include(GoogleTest)
	gtest_discover_tests(${TargetName})
endfunction()