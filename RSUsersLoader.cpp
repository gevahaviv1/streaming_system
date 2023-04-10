//
// Created by Geva on 08/01/2023.
//
#include <cmath>
#include "RSUsersLoader.h"
#define TOP 10
#define LOW 1

std::vector<sp_movie> get_movies (std::istringstream get_token);

std::vector<RSUser>
RSUsersLoader::create_users_from_file (const std::string &users_file_path,
                                       std::unique_ptr<RecommenderSystem> rs)
noexcept (false)
{
  std::ifstream file (users_file_path);
  if (file.fail ())
  { throw std::runtime_error ("Could not open file."); }

  std::string line, name, token;
  std::getline (file, line);
  std::vector<sp_movie> movies = get_movies (std::istringstream (line));
  std::vector<RSUser> users;
  std::shared_ptr<RecommenderSystem> shared_rs = std::move (rs);

  int i;
  while (std::getline (file, line))
  {
    std::istringstream get_token (line);
    rank_map rank (0, sp_movie_hash, sp_movie_equal);
    get_token >> name;
    i = 0;
    while (get_token >> token)
    {
      if (token != "NA")
      {
        if (std::stoi (token) <= TOP && std::stoi (token) >= LOW)
        { rank[movies[i]] = std::stoi (token); }
        else
        { throw std::out_of_range ("Out of range."); }
      }
      else
      {
        rank[movies[i]] = 0;
      }
      ++i;
    }

    users.emplace_back (name, rank, shared_rs);
  }
  return users;
}

std::vector<sp_movie> get_movies (std::istringstream get_token)
{
  std::vector<sp_movie> movies;
  std::string token, mv_name;
  int year;

  while (get_token >> token)
  {
    year = std::stoi (token.substr (token.rfind ('-') + 1));
    mv_name = token.substr (0, token.rfind ('-'));
    movies.push_back (std::make_shared<Movie> (mv_name, year));
  }

  return movies;
}