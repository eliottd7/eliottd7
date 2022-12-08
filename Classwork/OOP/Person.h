#include <string>

using namespace std;

class Person{
	public:
		int income;
		double tax_rate;
		Person* enemy;
		Person* buddy;
		string name;

		//constructor
		Person(int income, double tax_rate, string name);

		//calculates the post tax income. Takehome_pay = income - tax_rate * income
		int calc_takehome_pay();

		//Determines if P1 is an enemy or friend. If the Person makes the same or more
		//than P1, P1 is a friend. Else if the Person makes less than P1, P1 is an enemy.
		//Sets P1 to be the persons enemy or buddy based on this calculation.
		void enemy_or_buddy(Person* P1);
		//Prints out the Person’s enemy’s and buddy’s name if it has one, otherwise
		//prints "No buddy!" or "No enemy"

		void print_status();
};
