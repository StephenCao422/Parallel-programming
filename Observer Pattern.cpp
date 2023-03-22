#include <iostream>
#include <vector>
#include <mutex>
#include <thread>

// The Subject class maintains a list of observers and notifies them when the value changes
class Subject {
public:
    // Add an observer to the list
    void addObserver(Observer* observer) {
        std::lock_guard<std::mutex> lock(mutex_);
        observers_.push_back(observer);
    }

    // Remove an observer from the list
    void removeObserver(Observer* observer) {
        std::lock_guard<std::mutex> lock(mutex_);
        // Use the std::remove/erase idiom to remove the observer from the vector
        observers_.erase(std::remove(observers_.begin(), observers_.end(), observer), observers_.end());
    }

    // Notify all observers that the value has changed
    void notifyObservers(int value) {
        std::lock_guard<std::mutex> lock(mutex_);
        // Call the update() method on each observer in the list
        for (auto observer : observers_) {
            observer->update(value);
        }
    }

private:
    // The list of observers
    std::vector<Observer*> observers_;
    // A mutex to ensure thread safety when accessing the list of observers
    std::mutex mutex_;
};

// The Observer class is an abstract base class for all observers
class Observer {
public:
    // The constructor adds the observer to the subject's list
    Observer(Subject* subject) : subject_(subject) {
        subject_->addObserver(this);
    }

    // The destructor removes the observer from the subject's list
    virtual ~Observer() {
        subject_->removeObserver(this);
    }

    // This method is called by the subject when the value changes
    virtual void update(int value) = 0;

private:
    // The subject that the observer is observing
    Subject* subject_;
};

// The ValueObserver class is a concrete observer that simply prints the value when notified
class ValueObserver : public Observer {
public:
    ValueObserver(Subject* subject) : Observer(subject) {}

    void update(int value) override {
        std::cout << "ValueObserver: " << value << std::endl;
    }
};

// The main function demonstrates the observer pattern by creating a subject, adding an observer to it, and notifying the observer of a series of values
int main() {
    Subject subject;
    ValueObserver observer(&subject);
    std::thread thread1([&subject]() {
        for (int i = 0; i < 5; i++) {
            subject.notifyObservers(i);
        }
    });
    std::thread thread2([&subject]() {
        for (int i = 5; i < 10; i++) {
            subject.notifyObservers(i);
        }
    });
    thread1.join();
    thread2.join();
    return 0;
}

