#include <iostream>
#include <fstream>
#include <string>
#include <boost/json/src.hpp>

using namespace boost::json;

class dude
{
	std::string name;
	int age;
	int height;
public:
	//constructors
	dude()
	{
		name = ""; age = 0; height = 0;
	}
	dude(std::string vName, int vAge, int vHeight)
	{
		name = vName; age = vAge; height = vHeight;
	}
	//getters and setters
	void setName(std::string val) { name = val; }
	void setAge(int val) { age = val; }
	void setHeight(int val) { height = val; }
	std::string getName() { return name; }
	int getAge() { return age; }
	int getHeight() { return height; }
	void printData()
	{
		std::cout << "Dude is " << name.c_str() << "; age = " << age << "; height = " << height << ";\n";
	}
	void serializeFstream(std::string fileName);
	void deserializeFstream(std::string fileName);
	void serializeJSON(std::string fileName);
	void deserializeJSON(std::string fileName);
};

void dude::serializeFstream(std::string fileName)
{
	std::ofstream dudeSaver(fileName);
	dudeSaver << name.c_str() << ";" << age << ";" << height << ".";
	dudeSaver.close();
}

void dude::deserializeFstream(std::string fileName)
{

	std::ifstream dudeSaver(fileName);
	std::string dudeData="";
	if (dudeSaver.is_open())
	{
		getline(dudeSaver, dudeData);
		dudeSaver.close();
	}

	int l = dudeData.length();
	int i = 0;
	char cs = ' ';
	int field = 0;
	std::string buf = "";

	while ((i<l) && (cs!='.'))
	{		
		cs = dudeData[i];

		if ((cs != ';') && (cs != '.'))
		{			
			buf += cs;			
		}		
		else
		{
			if (field == 0)
				name = buf;
			if (field == 1)
				age = atoi(buf.c_str());
			if (field == 2)
				height = atoi(buf.c_str());
			buf = "";
			field++;
		}

		i++;
	}
}

void dude::serializeJSON(std::string fileName)
{
	//boost::json::value dudeVal =
	value dudeVal =
	{
		{"name",name},
		{"age",age},
		{"height",height}
	};

	serializer jsonSer;
	jsonSer.reset(&dudeVal);

	std::ofstream jsonWriter(fileName);

	while (!jsonSer.done())
	{
		char buffer[BOOST_JSON_STACK_BUFFER_SIZE];
		jsonWriter << jsonSer.read(buffer);
	}

	jsonWriter.close();
}

void dude::deserializeJSON(std::string fileName)
{
	std::ifstream jsonReader(fileName);
	std::string line = "";
	std::string fullText = "";
	while (getline(jsonReader, line))
	{
		fullText += line;
	}

	stream_parser parser;
	parser.reset();
	parser.write(fullText);
	value readValue = parser.release();

	name   = value_to<std::string>(readValue.at("name"));
	age    = value_to<int>(readValue.at("age"));
	height = value_to<int>(readValue.at("height"));

}


int main()
{
	std::cout << "Classic serialization\n";
	dude dude1("Gheorghe", 20, 180);
	dude dude2;

	dude1.printData();
	dude2.printData();

	dude1.serializeFstream("dude1data.dat");
	dude2.deserializeFstream("dude1data.dat");

	dude1.printData();
	dude2.printData();

	std::cout << "JSON serialization\n";
	dude dude3("Naftalinio", 40, 160);
	dude dude4;

	dude3.printData();
	dude4.printData();

	dude3.serializeJSON("dude3data.json");
	dude4.deserializeJSON("dude3data.json");

	dude3.printData();
	dude4.printData();
}