#include <sstream>
#include <iomanip>
#include "book.h"

using namespace std;

Book::Book(const std::string name, double price, int qty, const std::string isbn, const std::string author) : Product("book", name, price, qty) {
	isbn_ = isbn;
	author_ = author;
}
Book::~Book() {

}

    /**
     * Returns the appropriate keywords that this product should be associated with
     */
std::set<std::string> Book::keywords() const{
	std::set<std::string> bookKey = parseStringToWords(getAuthor());
	bookKey.insert(convToLower(getIsbn()));
	std::set<std::string> nameKey = parseStringToWords(getName());
	std::set<std::string> allSets = setUnion(bookKey, nameKey);
	return allSets;
}

    /**
     * Returns a string to display the product info for hits of the search
     */
std::string Book::displayString() const{
	std::ostringstream price;
	price << std::fixed << std::setprecision(2) << getPrice();
	std::string sPrice = price.str();
	std::string out = getName() + "\n" + "Author: " + getAuthor() + " ISBN: " + getIsbn() + "\n" + sPrice + " " + std::to_string(getQty()) + " left.";
	return out;
}

    /**
     * Outputs the product info in the database format
     */
void Book::dump(std::ostream& os) const{
	os << category_ << "\n" << name_ << "\n" << price_ << "\n" << qty_ << "\n" << isbn_ << "\n" << author_ << endl;
}

std::string Book::getIsbn() const {
	return isbn_;
}
std::string Book::getAuthor() const {
	return author_;
}