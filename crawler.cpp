#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <iostream>
#include <queue>
#include <regex>

#include "crawler.hpp"
#include "page_rank.hpp"

std::string Crawler::get_hash(std::string const & url_name)
{
  size_t hash = std::hash<std::string>{}(url_name);
  return std::to_string(hash);
}

int Crawler::fetch_page_text(std::string const & url, std::string const & write_here) const
{
  return std::system(("wget -O " + write_here + " " + url).c_str());
}

void Crawler::crawl()
{
  std::cout << "Crawling..." << std::endl;
  std::ifstream fin(CRAWL_SEED_SRC);
  std::queue<std::string> urls;

  if (!fin) {
      throw std::runtime_error{"Cannot open CRAWL_SEED_SRC file! Needs seed to start crawling"};
  }

  std::string url;
  while (fin >> url) {
      urls.push(url);
  }

  int curr_crawl_page = 0;
  std::ofstream fout(CRAWLED_IDS);

  PageRank pr;
  std::unordered_set<std::string> visited_ids;
  std::unordered_map<std::string, int> num_outbound;

  while (!urls.empty() && curr_crawl_page < CRAWL_NUM_PAGE) {
      std::string url = urls.front();
      urls.pop();

      if (visited.find(url) == visited.end()) {
          std::string url_id = get_hash(url);
          std::string write_here = std::string(CRAWLED) + "/" + url_id;
          if (fetch_page_text(url, write_here)) {
              std::cout << "Page NOT fetched " << url << "! ";
              if (remove(write_here.c_str())) {
                  std::cout << "Deleted it..." << std::endl;
              } else {
                  std::cout << "Not found / Couldn't delete it..." << std::endl;
              }
              continue;
          }

          std::cout << "Fetched " << url << std::endl;
          fout << url_id << " " << url << " ";

          std::ifstream fetch_file(write_here);
          std::string page;
          page.assign(std::istreambuf_iterator<char>(fetch_file), std::istreambuf_iterator<char>());
          fetch_file.close();

          std::ofstream put_file(write_here, std::ios::trunc);
          size_t ptr = 0;
          while (ptr < page.length()) {
              while (ptr++ < page.length() && page[ptr] != '<') {
                  put_file << page[ptr];
              }
              while (ptr++ < page.length() && page[ptr] != '>');
          }
          put_file.close();

          const std::regex link_expr("<a\\s+href=\"([\\-:\\w\\d\\.\\/]+)\">", std::regex_constants::icase);
          std::match_results<std::string::const_iterator> res;
          std::string::const_iterator start = page.begin(), end = page.end();
          int count_edges = 0;
          while (std::regex_search(start, end, res, link_expr)) {
              urls.push(res[1]);
              pr.add_edge(get_hash(res[1]), url_id);
              start = res[0].second;
              count_edges++;
          }

          visited.insert(url);
          visited_ids.insert(url_id);
          num_outbound[url_id] = count_edges;
          curr_crawl_page++;
      }
  }
  fout.close();
  pr.calculate_ranks(visited_ids, num_outbound);

  std::cout << "Crawling complete!!\n";
}
