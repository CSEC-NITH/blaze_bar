#define PAGE_RANKS "res/page_ranks"
#define MAX_ITER_PAGE_RANK 7

#include <set>
#include <string>
#include <unordered_set>
#include <unordered_map>

class PageRank
{
  private:
      std::unordered_map<std::string, std::set<std::string>> adj_lst;

  public:
      void add_edge(std::string const &, std::string const &);
      void calculate_ranks(std::unordered_set<std::string> const &, std::unordered_map<std::string, int> &);
};
