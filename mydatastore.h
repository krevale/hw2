#include <string>
#include <set>
#include <vector>
#include "product.h"
#include "user.h"
#include "datastore.h"

#include <map>

class MyDataStore : public DataStore {
public:
	MyDataStore() { }
	~MyDataStore();
    /**
     * Adds a product to the data store
     */
  void addProduct(Product* p) override;

    /**
     * Adds a user to the data store
     */
  void addUser(User* u) override;

    /**
     * Performs a search of products whose keywords match the given "terms"
     *  type 0 = AND search (intersection of results for each term) while
     *  type 1 = OR search (union of results for each term)
     */
  std::vector<Product*> search(std::vector<std::string>& terms, int type) override;

    /**
     * Reproduce the database file from the current Products and User values
     */
  void dump(std::ostream& ofile) override;

	User* returnUser(std::string username_) const;

	bool userExists(std::string username_) const;

private:
	std::map<std::string, std::set<Product*>> products_;
	std::map<std::string, User*> users_;
	std::set<Product*> allProducts_;
};