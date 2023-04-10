//
// Created by Geva on 08/01/2023.
//

#include "RecommenderSystemLoader.h"
#define TOP 10
#define LOW 1

std::unique_ptr<RecommenderSystem>
RecommenderSystemLoader::create_rs_from_movies_file
    (const std::string &movies_file_path) noexcept (false)
{
  std::unique_ptr<RecommenderSystem> rs_data =
      std::make_unique<RecommenderSystem> ();
  std::ifstream file (movies_file_path);
  if (file.fail ())
  { throw std::runtime_error ("Could not open file."); }

  std::string name, line, token;
  int year;

  while (std::getline (file, line))
  {
    std::istringstream get_token (line);
    get_token >> token;

    year = std::stoi (token.substr (token.rfind ('-') + 1));
    name = token.substr (0, token.rfind ('-'));

    // Push scores to the vector.
    std::vector<double> vec;
    while (get_token >> token)
    {
      int rate = std::stoi (token);
      if (rate > TOP || rate < LOW)
      {
        throw std::out_of_range ("Out of range.");
      }
      vec.push_back (std::stoi (token));
    }


    rs_data->add_movie (name, year, vec);
  }
  return rs_data;
}
