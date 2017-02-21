#### BASIC USAGE

```C++
#include <fstream>
#include <wss>

//

int main(int argc,char* argv[])
{
	wss::Settings settings;
	
	// Reading
	
	std::ifstream in("Settings.wss",std::ios::binary);
	
	settings.Read(in);
	
	std::cout << settings["Audio"]["MusicVolume"] << std::endl;
	
	// Writing
	
	settings["Graphics"]["Quality"] = "Ultra";
	
	std::ofstream out("Settings.wss",std::ios::binary | std::ios::trunc);
	
	settings.Write(out);
	
	// Misc
	
	wss::Box& box = settings["Input"];
	wss::Pair& pair = box["Forward"];
	
	pair = "W";
	
	std::cout << box.GetName() << " > " << pair.GetName() << " = " << pair << std::endl;
	
	return 0;
}
```

#### FILE SYNTAX

```
-- This is a comment line

-- Boxes are like categories and start with #
-- Every following pair belongs to this box

#Graphics

Width 1920
Height 1080
Fullscreen 1
Quality Ultra
```
