#include <fstream>
#include <iostream>

#include "page_rank.hpp"

void PageRank::add_edge(std::string const & src, std::string const & dest)
{
  adj_lst[src].insert(dest);
}

void PageRank::calculate_ranks(std::unordered_set<std::string> const & visited, std::unordered_map<std::string, int> & num_outbound)
{
  const double damping_factor = 0.85;
  double initial_page_rank = 1.0 / visited.size();
  std::unordered_map<std::string, std::pair<double, double>> rank_list;     // pair.first --> original rank, pair.second --> temporary rank
  for (auto & node : visited) {
      rank_list.emplace(std::make_pair(node, std::make_pair(initial_page_rank, initial_page_rank)));
  }

  for (int i = 0; i < MAX_ITER_PAGE_RANK; i++) {
      for (auto & page : adj_lst) {
          if (visited.find(page.first) != visited.end()) {
              double pr = 1 - damping_factor;
              for (auto & link : page.second) {
                  pr += damping_factor * (rank_list[link].first / num_outbound[link]);
              }
              rank_list[page.first].second = pr;
          }
      }

      for (auto & page_rank : rank_list) {
          page_rank.second.first = page_rank.second.second;
      }
  }

  std::ofstream fout(PAGE_RANKS);
  if (fout) {
      for (auto & page_rank : rank_list) {
          fout << page_rank.first << " " << page_rank.second.first << " ";
      }
      fout.close();
  } else {
      std::cerr << "WARNING: Cannot save page ranks! Will have to resort to query based ranks!\n";
  }
}
