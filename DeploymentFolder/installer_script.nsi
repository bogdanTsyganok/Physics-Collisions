# This example is from https://nsis.sourceforge.io/Simple_tutorials

# This installer script will copy a file "test.txt"
# to the installation directory
#
# First, create the text.txt file in the same directory as
# the installer script below then compile the installer script.
#
# Running the installer installs the test.txt file to the Desktop
# along with an uninstaller


# define the name of our installer
OutFile "pinball_installer.exe"

# define the directory to install to.
InstallDir "$DESKTOP\Pinball"

# start our default section
Section

# define the output path for this file
SetOutPath $INSTDIR

# define what to install and put in the output path
File /r "assets"


File /r "configs"

File BigYarn.exe

# define uninstaller
WriteUninstaller "$INSTDIR\pinball_uninstaller.exe"

# end our default section
SectionEnd

#no start our uninstaller section
# the section will always be "Uninstall"
Section "Uninstall"

 Delete "$INSTDIR\BigYarn.exe"
 
 Delete "$INSTDIR\assets\shaders\*.*"
 
 Delete "$INSTDIR\assets\*.*"
 
 Delete "$INSTDIR\configs\*.*"
 
 Delete "$INSTDIR\pinball_uninstaller.exe"


# delete the directory
RMDir "$INSTDIR\assets\shaders"
RMDir "$INSTDIR\assets"
RMDir "$INSTDIR\configs"
RMDir $INSTDIR

# end the uninstaller section
SectionEnd