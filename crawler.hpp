#define CRAWLED_IDS "res/crawled_ids"
#define CRAWLED "res/crawled_dir"
#define CRAWL_NUM_PAGE 256
#define CRAWL_SEED_SRC "res/crawl_seed"
#define MAX_ITER_PAGE_RANK 7

#include <string>
#include <set>
#include <unordered_map>
#include <unordered_set>

class Crawler
{
  private:
      std::unordered_set<std::string> visited;

  public:
      std::string get_id(std::string const &);
      std::string get_hash(std::string const &);
      int fetch_page_text(std::string const &, std::string const &) const;
      void crawl();
};
