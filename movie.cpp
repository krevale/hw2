#include <sstream>
#include <iomanip>
#include "movie.h"

using namespace std;

Movie::Movie(const std::string name, double price, int qty, const std::string genre, const std::string rating) : Product("movie", name, price, qty) {
	genre_ = genre;
	rating_ = rating;
}
Movie::~Movie() {

}

    /**
     * Returns the appropriate keywords that this product should be associated with
     */
std::set<std::string> Movie::keywords() const{
	std::set<std::string> MovieKey = parseStringToWords(getGenre());
	MovieKey.insert(convToLower(getGenre()));
	std::set<std::string> nameKey = parseStringToWords(getName());
	std::set<std::string> allSets = setUnion(MovieKey, nameKey);
	return allSets;
}

    /**
     * Returns a string to display the product info for hits of the search
     */
std::string Movie::displayString() const{
	std::ostringstream price;
	price << std::fixed << std::setprecision(2) << getPrice();
	std::string sPrice = price.str();
	std::string out = getName() + "\n" + "Genre: " + getGenre() + " Rating: " + getRating() + "\n" + sPrice + " " + std::to_string(getQty()) + " left.";
	return out;
}

    /**
     * Outputs the product info in the database format
     */
void Movie::dump(std::ostream& os) const{
	os << category_ << "\n" << name_ << "\n" << price_ << "\n" << qty_ << "\n" << genre_ << "\n" << rating_ << endl;
}

std::string Movie::getGenre() const {
	return genre_;
}
std::string Movie::getRating() const {
	return rating_;
}