#include <sstream>
#include <iomanip>
#include "clothing.h"

using namespace std;

Clothing::Clothing(const std::string name, double price, int qty, const std::string size, const std::string brand) : Product("clothing", name, price, qty) {
	size_ = size;
	brand_ = brand;
}
Clothing::~Clothing() {

}

    /**
     * Returns the appropriate keywords that this product should be associated with
     */
std::set<std::string> Clothing::keywords() const{
	std::set<std::string> ClothingKey = parseStringToWords(getBrand());

	std::set<std::string> nameKey = parseStringToWords(getName());
	std::set<std::string> allSets = setUnion(ClothingKey, nameKey);
	return allSets;
}

    /**
     * Returns a string to display the product info for hits of the search
     */
std::string Clothing::displayString() const{
	std::ostringstream price;
	price << std::fixed << std::setprecision(2) << getPrice();
	std::string sPrice = price.str();
	std::string out = getName() + "\n" + "Size: " + getSize() + " Brand: " + getBrand() + "\n" + sPrice + " " + std::to_string(getQty()) + " left.";
	return out;
}

    /**
     * Outputs the product info in the database format
     */
void Clothing::dump(std::ostream& os) const{
	os << category_ << "\n" << name_ << "\n" << price_ << "\n" << qty_ << "\n" << size_ << "\n" << brand_ << endl;
}

std::string Clothing::getSize() const {
	return size_;
}
std::string Clothing::getBrand() const {
	return brand_;
}