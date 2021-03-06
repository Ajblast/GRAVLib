function(setMSVCRuntime TargetName DynamicRuntime)
	#Should the lib be compiled with a dynamic msvc runtime or static
	if(DynamicRuntime)
		set_property(TARGET ${TargetName} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
	else()
		set_property(TARGET ${TargetName} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
	endif()
endfunction()

function(makelib LibName LibPath LibType DynamicRuntime pchName BuildTests LinkLibraries)
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
	target_link_libraries(${LibName} PUBLIC ${LinkLibraries})

	# Is the lib a static or dynamic lib
	if(${LibType} STREQUAL SHARED)
		target_compile_definitions(${LibName} PRIVATE "${LibName}_EXPORTS")
	elseif(${LibType} STREQUAL STATIC)
	endif()

	#Should the exe be compiled with a dynamic msvc runtime or static
	setmsvcruntime(${TargetName} ${DynamicRuntime})

	#Print debug info about the project
	if(GRAVLib_DebugInfo)
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
		message("Link Libraries: " ${LinkLibraries})

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

function(makeexe ExeName ExePath DynamicRuntime pchName LinkLibraries)
	set(ExeName ${ExeName} PARENT_SCOPE)
	set(TargetName ${ExeName})

	set(IncludePath ${ExePath}/include)
	set(SourcePath ${ExePath}/src)

	#Create the Exerary
	add_executable(${ExeName})
	add_subdirectory(${IncludePath})
	add_subdirectory(${SourcePath})
	target_precompile_headers(${ExeName} PRIVATE ${pchName})
	target_include_directories(${ExeName} PUBLIC ${IncludePath})
	target_link_libraries(${ExeName} PUBLIC ${LinkLibraries})

	#Should the exe be compiled with a dynamic msvc runtime or static
	setmsvcruntime(${TargetName} ${DynamicRuntime})

	#Print debug info about the project
	if(GRAVLib_DebugInfo)
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
		message("Link Libraries: " ${LinkLibraries})

		if(BuildTypes)
			message("")
			message("Build Type Compiler Flags")
			foreach(BuildType ${BuildTypes})
				message("${BuildType} Flags: ${CMAKE_${ProjectLanguage}_FLAGS_${BuildType}}")
			endforeach()
		endif()
	endif()
endfunction()

function(maketest TargetName DynamicRuntime LinkLibraries)
	if(GRAVLib_DebugInfo)
		message("")
		message("Make Tests: ${TargetName}")
		message("Dynamic Runtime: ${DynamicRuntime}")
		#message("Source Files: ${SourceFiles}")
		message("Link Libraries: ${LinkLibraries}")
	endif()

	set(TestName ${TargetName} PARENT_SCOPE)

	add_executable(${TargetName})
	include(GoogleTest)
	target_link_libraries(${TargetName} GTest::gtest_main ${LinkLibraries})
	
	setmsvcruntime(${TargetName} ${DynamicRuntime})

	gtest_discover_tests(${TargetName})
endfunction()