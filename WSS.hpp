
/*

MIT License

Copyright (c) 2017 

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#pragma once

//

#include <fstream>
#include <string>
#include <vector>

//

#define WSC_PRIVATE private:
#define WSC_PUBLIC public:

//

namespace wss
{
	class Pair
	{
		friend class Box;

		WSC_PRIVATE

		std::string m_name;
		std::string m_value;

		WSC_PUBLIC

		inline Pair(const char* name);
		inline Pair(const Pair& o);

		inline const char* GetName() const;

		inline Pair& operator=(const char* str);
		inline Pair& operator=(const std::string& str);
		inline Pair& operator=(const int32_t& val);
		inline Pair& operator=(const uint32_t& val);
		inline Pair& operator=(const float& val);
		inline Pair& operator=(const bool& val);

		inline operator const char*() const;
		inline operator int32_t() const;
		inline operator uint32_t() const;
		inline operator float() const;
		inline operator bool() const;

		inline Pair& operator=(const Pair& o);
	};

	//

	Pair::Pair(const char* name)
	{
		m_name = name;
	}

	//

	Pair::Pair(const Pair& o)
	{
		m_name = o.m_name;
		m_value = o.m_value;
	}

	//

	const char* Pair::GetName() const
	{
		return m_name.c_str();
	}

	//

	Pair& Pair::operator=(const char* str)
	{
		m_value = str;

		return *this;
	}

	//

	Pair& Pair::operator=(const std::string& str)
	{
		m_value = str;

		return *this;
	}

	//

	Pair& Pair::operator=(const int32_t& val)
	{
		m_value = std::to_string(val);

		return *this;
	}

	//

	Pair& Pair::operator=(const uint32_t& val)
	{
		m_value = std::to_string(val);

		return *this;
	}

	//

	Pair& Pair::operator=(const float& val)
	{
		m_value = std::to_string(val);

		return *this;
	}

	//

	Pair& Pair::operator=(const bool& val)
	{
		m_value = (val ? "1": "0");

		return *this;
	}

	//

	Pair::operator const char*() const
	{
		return m_value.c_str();
	}

	//

	Pair::operator int32_t() const
	{
		return static_cast<int32_t>(std::atol(m_value.c_str()));
	}

	//

	Pair::operator uint32_t() const
	{
		return static_cast<uint32_t>(std::atol(m_value.c_str()));
	}

	//

	Pair::operator float() const
	{
		return static_cast<float>(std::atof(m_value.c_str()));
	}

	//

	Pair::operator bool() const
	{
		return (m_value[0] == '1');
	}

	//

	Pair& Pair::operator=(const Pair& o)
	{
		m_name = o.m_name;
		m_value = o.m_value;

		return *this;
	}

	//

	class Box
	{
		friend class Settings;

		WSC_PRIVATE

		std::string m_name;
		std::vector<Pair> m_pairs;

		WSC_PUBLIC

		inline Box(const char* name);
		inline Box(const Box& o);

		inline const char* GetName() const;

		inline Pair& operator[](const char* name);
		inline Pair& operator[](const std::string& name);

		inline Box& operator=(const Box& o);
	};

	//

	Box::Box(const char* name)
	{
		m_name = name;
	}

	//

	Box::Box(const Box& o)
	{
		m_name = o.m_name;
		m_pairs = o.m_pairs;
	}

	//

	const char* Box::GetName() const
	{
		return m_name.c_str();
	}

	//

	Pair& Box::operator[](const char* name)
	{
		const std::size_t size = m_pairs.size();

		for(std::size_t i = 0; i < size; ++i)
		{
			if(m_pairs[i].m_name == name) return m_pairs[i];
		}

		m_pairs.emplace_back(name);

		return m_pairs.back();
	}

	//

	Pair& Box::operator[](const std::string& name)
	{
		return (*this)[name.c_str()];
	}

	//

	Box& Box::operator=(const Box& o)
	{
		m_name = o.m_name;
		m_pairs = o.m_pairs;

		return *this;
	}

	//

	class Settings
	{
		WSC_PRIVATE

		std::vector<Box> m_boxes;

		WSC_PUBLIC

		inline Settings();
		inline Settings(const Settings& o);

		inline void Read(std::ifstream& in);
		inline void Write(std::ofstream& out);

		inline Box& operator[](const char* name);
		inline Box& operator[](const std::string& name);

		inline Settings& operator=(const Settings& o);
	};

	//

	Settings::Settings()
	{
	}

	//

	Settings::Settings(const Settings& o)
	{
		m_boxes = o.m_boxes;
	}

	//

	void Settings::Read(std::ifstream& in)
	{
		Box* box = nullptr;

		std::string line;

		std::size_t pos;

		while(std::getline(in,line))
		{
			if(line.size() < 2) continue;
			if(line[0] == '-' && line[1] == '-') continue;

			if(line[0] == '#')
			{
				box = &((*this)[line.c_str() + 1]);
				continue;
			}
			
			if(box == nullptr) continue;

			pos = line.find_first_of(' ');

			if(pos == std::string::npos) continue;

			line[pos++] = '\0';

			(*box)[line.c_str()] = (line.c_str() + pos);
		}
	}

	//

	void Settings::Write(std::ofstream& out)
	{
		const std::size_t bsize = m_boxes.size();

		for(std::size_t i = 0; i < bsize; ++i)
		{
			const Box& box = m_boxes[i];

			out << "\n#" << box.m_name << "\n";

			const std::size_t psize = box.m_pairs.size();

			for(std::size_t j = 0; j < psize; ++j)
			{
				const Pair& pair = box.m_pairs[j];

				out << pair.GetName() << " " << static_cast<const char*>(pair) << "\n";
			}
		}
	}

	//

	Box& Settings::operator[](const char* name)
	{
		const std::size_t size = m_boxes.size();

		for(std::size_t i = 0; i < size; ++i)
		{
			if(m_boxes[i].m_name == name) return m_boxes[i];
		}

		m_boxes.emplace_back(name);

		return m_boxes.back();
	}

	//

	Box& Settings::operator[](const std::string& name)
	{
		return (*this)[name.c_str()];
	}

	//

	Settings& Settings::operator=(const Settings& o)
	{
		m_boxes = o.m_boxes;

		return *this;
	}
}
