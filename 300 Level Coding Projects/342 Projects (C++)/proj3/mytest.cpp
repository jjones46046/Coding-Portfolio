
#include "pqueue.h"
using namespace std;

int priorityFn1(const Patient &patient);
int priorityFn2(const Patient &patient);

int main() {
  // Create a patient queue with capacity five and priority function
  //priorityFn1()

    PQueue queue1(priorityFn1);
    PQueue queue2(priorityFn1);
    PQueue queue3(priorityFn1);
    PQueue queue4(priorityFn1); //Used for invalid merge request testing.

    //Tests for empty queue errors:
    /*
        cout << "Testing empty queue domain_error calls." << endl;
       queue1.getNextPatient();
       queue1.dump();
    */ 

  // Create some test jobs and insert them into the queue
  //Patient(string name, int triage, int temp, int ox, int rr, int hr, int bp)
  Patient patient1("Sam", 1, 37, 79,  36, 133, 75);
  Patient patient2("Liz", 5, 37, 98, 26,  70, 100);
  Patient patient3("Nick", 2, 37, 85, 34, 125, 85);
  
  Patient patient4("Eva", 4, 37, 95,  22, 115, 95);
  Patient patient5("Mark", 2, 37, 85, 40, 120, 90);
  Patient patient6("Ty", 2, 37, 88, 35, 113, 99);
  cout << "Inserting..." << endl;

  queue1.insertPatient(patient1);
  queue1.insertPatient(patient3);
  queue1.insertPatient(patient4);
  
  queue1.insertPatient(patient2);

  cout << "Queue 1 original:" << endl;
  queue1.printPatientQueue();
  
  queue2.insertPatient(patient5);
  queue2.insertPatient(patient6);
  
  queue1.mergeWithQueue(queue2);
  cout << "--------------------------" << endl;
  cout << "Merge between queue1 and queue 2" << endl;
  
  queue1.dump();
  cout << "--------------------------" << endl;
  
  cout << "Removed Patient: " << queue1.getNextPatient() << endl;
  cout << "Removed Patient: " << queue1.getNextPatient() << endl;
  cout << "--------------------------" << endl;
  cout << "queue 1 after removing two patients" << endl;
  
  queue1.dump();
  cout << "--------------------------" << endl;
  
  cout << "Testing copy constructor" << endl;
  queue3 = queue1;
  queue3.dump();

  cout << "--------------------------" << endl;

  cout << "Number patients for queue3 and queue1: (should be the same)" << endl;
 
  cout << "queue1: " << queue1.numPatients() << endl;
  cout << "queue3: " << queue3.numPatients() << endl;
  
  //Test cases for invalid merge request:
  /*
    cout << "Testing invalid queue merge request." << endl;
    
    queue1.mergeWithQueue(queue4);
    queue1.mergeWithQueue(queue1);
    //Should output domain error.
  */
  return 0; 
}


int priorityFn1(const Patient&patient) {
  int pri = 5;
  if ((patient.getOxygen()<80 && (patient.getRR() > 35 || patient.getRR() < 8)) ||
      (patient.getHR() > 130 && patient.getBP() < 80))
    pri = 1;
  else if (((patient.getOxygen() >= 80 && patient.getOxygen() <= 89) &&
	    (patient.getRR() >= 31 && patient.getRR() <= 35)) ||
	   (patient.getTemperature() >= 40 || patient.getTemperature() <= 32))
    pri = 2;
  else if ((patient.getOxygen() >= 90 && patient.getOxygen() <= 94) &&
	   (patient.getRR() >= 26 && patient.getRR() <= 30))
    pri = 3;
  else if ((patient.getOxygen() >= 95 && patient.getOxygen() <= 97) &&
	   (patient.getRR() >= 21 && patient.getRR() <= 25))
    pri = 4;
  else
    pri = 5;
  return pri;
}

int priorityFn2(const Patient&patient) {
  //priority only based on triage
  //traige is set by a health care professional
  return patient.getTriage();
}
