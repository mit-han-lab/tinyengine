# This will sort the Eclipse indexer issue with mixed C/C++ source code projects
# by Wolfgang Schmieder
# (https://www.eclipse.org/forums/index.php/t/1066506/?)

# Create language specific (C/C++) compiler built in macros and include paths
message("${MESSAGE_TABS}language_provider.cmake ...")
SET(MESSAGE_TABS "${MESSAGE_TABS}\t")

#Try replacing in function create_language_provider the text:
# 'org.eclipse.cdt.managedbuilder.language.settings.providers.GCCBuiltinSpecsDetector' by
# 'org.eclipse.cdt.managedbuilder.internal.language.settings.providers.GCCBuiltinSpecsDetectorCygwin' if you use Cygwin toolchain, respectively by
# 'org.eclipse.cdt.managedbuilder.internal.language.settings.providers.GCCBuiltinSpecsDetectorMinGW' if you use MinGW toolchain.
# Note: The above settings for Cygwin / MinGW have not been tested yet.

function(create_language_provider OUTPUT_DIR COMMAND_PREFIX)
set(OUTPUT_FILE ${OUTPUT_DIR}/language.settings.xml)
file(WRITE ${OUTPUT_FILE} "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n")
file(APPEND ${OUTPUT_FILE} "<project>\n")
file(APPEND ${OUTPUT_FILE} "\t<configuration id=\"org.eclipse.cdt.core.default.config.1\" name=\"Configuration\">\n")
file(APPEND ${OUTPUT_FILE} "\t\t<extension point=\"org.eclipse.cdt.core.LanguageSettingsProvider\">\n")
file(APPEND ${OUTPUT_FILE} "\t\t\t<provider copy-of=\"extension\" id=\"org.eclipse.cdt.ui.UserLanguageSettingsProvider\"/>\n")
file(APPEND ${OUTPUT_FILE} "\t\t\t<provider-reference id=\"org.eclipse.cdt.core.ReferencedProjectsLanguageSettingsProvider\" ref=\"shared-provider\"/>\n")
file(APPEND ${OUTPUT_FILE} "\t\t\t<provider class=\"org.eclipse.cdt.managedbuilder.language.settings.providers.GCCBuiltinSpecsDetector\" console=\"false\" id=\"org.eclipse.cdt.managedbuilder.core.GCCBuiltinSpecsDetector\" keep-relative-paths=\"false\" name=\"CDT GCC Built-in Compiler Settings\" parameter=\"${COMMAND_PREFIX}\${COMMAND} \${FLAGS} ${ARGV2} -E -P -v -dD &quot;\${INPUTS}&quot;\" store-entries-with-project=\"true\">\n")
file(APPEND ${OUTPUT_FILE} "\t\t\t\t<language-scope id=\"org.eclipse.cdt.core.gcc\"/>\n")
file(APPEND ${OUTPUT_FILE} "\t\t\t\t<language-scope id=\"org.eclipse.cdt.core.g++\"/>\n")
file(APPEND ${OUTPUT_FILE} "\t\t\t</provider>\n")
file(APPEND ${OUTPUT_FILE} "\t\t\t<provider-reference id=\"org.eclipse.cdt.core.PathEntryScannerInfoLanguageSettingsProvider\" ref=\"shared-provider\"/>\n")
file(APPEND ${OUTPUT_FILE} "\t\t</extension>\n")
file(APPEND ${OUTPUT_FILE} "\t</configuration>\n")
file(APPEND ${OUTPUT_FILE} "</project>\n")
endfunction(create_language_provider)


macro(remove_from_SYSTEM_ITEM LANGUAGE ITEM_TYPE CACHE_DOC_STRING)
# message("${MESSAGE_TABS}CMAKE_EXTRA_GENERATOR_${LANGUAGE}${ITEM_TYPE} = ${CMAKE_EXTRA_GENERATOR_${LANGUAGE}${ITEM_TYPE}}")
set(TEMP_VAR)
if(${ARGC} LESS 4)
# There is no regular expression, so remove everything
message("${MESSAGE_TABS}wiping ${CACHE_DOC_STRING}")
else(${ARGC} LESS 4)
# The extra argument ist treated as an regular expression. Every match will be removed.
message("${MESSAGE_TABS}Removing ${ARGN} from ${CACHE_DOC_STRING}")

set(TEMP_VAR "${CMAKE_EXTRA_GENERATOR_${LANGUAGE}${ITEM_TYPE}}")
STRING(REGEX REPLACE "${ARGN}" "" TEMP_VAR "${TEMP_VAR}")
endif(${ARGC} LESS 4)
set(CMAKE_EXTRA_GENERATOR_${LANGUAGE}${ITEM_TYPE} "${TEMP_VAR}" CACHE INTERNAL "${CACHE_DOC_STRING}")
# message("${MESSAGE_TABS}CMAKE_EXTRA_GENERATOR_${LANGUAGE}${ITEM_TYPE} = ${CMAKE_EXTRA_GENERATOR_${LANGUAGE}${ITEM_TYPE}}")
endmacro(remove_from_SYSTEM_ITEM ITEM_TYPE)

# Remove a string from C or CXX system include directories.
# The string to be removed is passed as a regular expression within the optional the last argument
# If there is no regular expression, ALL system include directories will be removed.
macro(remove_from_SYSTEM_INCLUDE_DIRS LANGUAGE)
set(system_DOC_STRING "${LANGUAGE} compiler system include directories")
remove_from_SYSTEM_ITEM(${LANGUAGE} _SYSTEM_INCLUDE_DIRS "${system_DOC_STRING}" ${ARGN})
endmacro(remove_from_SYSTEM_INCLUDE_DIRS)

# Remove a string from C or CXX system defined macros.
# The string to be removed is passed as a regular expression within the optional the last argument
# If there is no regular expression, ALL system defined macros will be removed.
macro(remove_from_SYSTEM_MACROS LANGUAGE)
set(system_DOC_STRING "${LANGUAGE} compiler system defined macros")
remove_from_SYSTEM_ITEM(${LANGUAGE} _SYSTEM_DEFINED_MACROS "${system_DOC_STRING}" ${ARGN})
endmacro(remove_from_SYSTEM_MACROS)

if(NOT ${MESSAGE_TABS} STREQUAL "")
STRING(SUBSTRING ${MESSAGE_TABS} 1 -1 MESSAGE_TABS)
endif(NOT ${MESSAGE_TABS} STREQUAL "")
message("${MESSAGE_TABS}language_provider.cmake done.\n")