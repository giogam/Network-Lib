#include "../../../include/phone_db.h"


phone_db::phone_db(std::string db_name)
{
  std::fstream inFile;
  std::string line;
  inFile.open(db_name.c_str());

    while(std::getline(inFile, line))
    {
      std::stringstream s_stream(line);
      std::string buf;
      std::vector<std::string> tokens;
      /* tokenizzo la stringa e inserisco la entry nella contactList*/
      while (s_stream >> buf) tokens.push_back(buf);
      contactList.insert(std::make_pair(tokens[0],listEntry(tokens[1],tokens[2])));
    }
}

std::vector<std::string> phone_db::search(std::string param)
{
  std::vector<std::string> result;
  std::string newString;
  
    /* Cerco gli estremi del range di valori uguali param */
    auto search = contactList.equal_range(param);
    for (auto it = search.first; it!=search.second; it++)
    {
      newString += it->first;
      newString += ";";
      newString += it->second.first;
      newString += ";";
      newString += it->second.second;
      newString += "\n";
      result.push_back(newString);
      newString.clear();
    }

    return result;
}
