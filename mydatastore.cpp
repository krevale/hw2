#include "mydatastore.h"
#include "util.h"

using namespace std;

MyDataStore::~MyDataStore() {
	for (std::map<std::string, User*>::iterator it = users_.begin(); it != users_.end(); ++it) {
		delete it->second;
	}

	users_.clear();

	for (set<Product*>::iterator it = allProducts_.begin(); it != allProducts_.end(); ++it) {
		delete *it;
	}

	allProducts_.clear();
	products_.clear();
}

void MyDataStore::addProduct(Product* p) {
	allProducts_.insert(p);
	for (const std::string& s : p->keywords()) {
		products_[s].insert(p);
	}
}

void MyDataStore::addUser(User* u) {
	users_.insert({u->getName(), u});
}

		/**
     * Performs a search of products whose keywords match the given "terms"
     *  type 0 = AND search (intersection of results for each term) while
     *  type 1 = OR search (union of results for each term)
     */
vector<Product*> MyDataStore::search(std::vector<std::string>& terms, int type) {
	vector<Product*> hits;

	if (terms.empty()) {
		return hits;
	}

	set<string> termSet(terms.begin(), terms.end());

	set<Product*> matches;

	if (type == 0) {
		//and search
		bool started = false;

		for (set<string>::iterator it = termSet.begin(); it!= termSet.end(); ++it) {
			map<string, set<Product*>>::iterator it2 = products_.find(*it);
			if (it2 == products_.end()) {
				matches.clear();
				break;
			}

			if (!started) {
				matches = it2->second;
				started = true;
			} else {
				matches = setIntersection(matches, it2->second);
				if (matches.empty()) {
					break;
				}
			}
		}

	} else {
		//or search
		for (set<string>::iterator it = termSet.begin(); it != termSet.end(); ++it) {
			map<string, set<Product*>>::iterator it2 = products_.find(*it);
			if (it2 != products_.end()) {
				matches = setUnion(matches, it2->second);
			}
		}
	}

	hits.assign(matches.begin(), matches.end());

	return hits;
}

void MyDataStore::dump(std::ostream& ofile) {
	ofile << "<products>" << endl;

	for (set<Product*>::iterator outProduct = allProducts_.begin(); outProduct != allProducts_.end(); ++outProduct) {
		(*outProduct)->dump(ofile);
	}

	ofile << "</products>" << endl;

	ofile << "<users>" << endl;

	for (map<string,User*>::iterator outUser = users_.begin(); outUser != users_.end(); ++outUser) {
		if (outUser->second) {
			outUser->second->dump(ofile);
		}
	}

	ofile << "</users>" << endl;
}

User* MyDataStore::returnUser(std::string username_) const {
	map<string, User*>::const_iterator it = users_.find(username_);
	if (it == users_.end()) {
		return nullptr;
	}
	return it->second;
}

bool MyDataStore::userExists(std::string username_) const {
	return (users_.find(username_) != users_.end());
}