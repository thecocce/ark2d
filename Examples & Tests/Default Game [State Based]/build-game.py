import sys
import os
import subprocess
import json 

if __name__ == "__main__":
	a = dict();
	a['ark2d_dir'] = "C:\\xampp\\htdocs\\C++\\ARK2D\\ARK2D";
	
	a['game_name'] = "ARK2D Default Game [State Based]";
	a['game_dir'] = "C:\\xampp\\htdocs\\C++\\ARK2D\\Examples & Tests\\Default Game [State Based]";
	a['game_src_files'] = [
		"src\\main.cpp", 
		"src\\DefaultGame.cpp", 
		"src\\BlankState.cpp", 
		"src\\FailState.cpp", 
		"src\\InGameState.cpp", 
		"src\\InstructionsState.cpp", 
		"src\\MenuState.cpp",
		"src\\WinState.cpp"
	];
	
	b = json.dumps(a, separators=(',',':')).replace(" ", "-");
	
	subprocess.call(["C:\Python31\python.exe ../../ARK2D/build.py " + b + ""], shell=True);