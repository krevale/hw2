#include <iostream>
#include <string>
#include <set>
#include <vector>
#include "product.h"
#include "util.h"

class Movie : public Product {
public:
  Movie(const std::string name, double price, int qty, const std::string genre, const std::string rating);
  ~Movie();

    /**
     * Returns the appropriate keywords that this product should be associated with
     */
  std::set<std::string> keywords() const override;

    /**
     * Returns a string to display the product info for hits of the search
     */
  std::string displayString() const override;

    /**
     * Outputs the product info in the database format
     */
  void dump(std::ostream& os) const override;

	std::string getGenre() const;
	std::string getRating() const;

private:
	std::string genre_;
	std::string rating_;
};