#include <iostream>
#include <string>
#include <utility>

using namespace std;

// Abstract base class for polymorphic media items (e.g., Song*, Podcast*, Audiobook*)
class Media {
public:
    virtual ~Media() = default;
    virtual void play() const = 0;
    virtual string getTitle() const = 0;
};

// Custom data type derived from Media
class Song : public Media {
private:
    string title;
    string artist;
    int durationSeconds;

public:
    Song(string title, string artist, int durationSeconds)
        : title(std::move(title)), artist(std::move(artist)), durationSeconds(durationSeconds) {}

    void play() const override {
        cout << "Playing Song: \"" << title << "\" by " << artist
             << " (" << durationSeconds << "s)" << endl;
    }

    string getTitle() const override {
        return title;
    }
};

// Templated Node storing a pointer of type T*
template <typename T>
struct Node {
    T* data;
    Node<T>* next;

    explicit Node(T* item) : data(item), next(nullptr) {}
};

// Templated Circular Linked List storing T* pointers inside each Node
template <typename T>
class LinkedList {
private:
    Node<T>* head;
    Node<T>* tail;
    Node<T>* current;

public:
    LinkedList() : head(nullptr), tail(nullptr), current(nullptr) {}

    // Insert at the end while maintaining circularity (tail->next == head)
    void insert(T* item) {
        Node<T>* newNode = new Node<T>(item);

        if (head == nullptr) {
            head = newNode;
            tail = newNode;
            newNode->next = head;
            current = head;
        } else {
            tail->next = newNode;
            tail = newNode;
            tail->next = head;
        }
    }

    // Traverse from head around the circle until wrapping back to head
    void displayList() const {
        if (head == nullptr) {
            cout << "[Playlist is empty]" << endl;
            return;
        }

        cout << "--- Circular Playlist ---" << endl;
        Node<T>* temp = head;
        int index = 1;
        do {
            cout << index++ << ". ";
            if (temp->data != nullptr) {
                temp->data->play();
            }
            temp = temp->next;
        } while (temp != head);
        cout << "-------------------------" << endl;
    }

    // Advance current pointer one step, wrapping from tail back to head
    void playNext() {
        if (head == nullptr || current == nullptr) {
            cout << "[No media available to play]" << endl;
            return;
        }

        if (current->data != nullptr) {
            current->data->play();
        }
        current = current->next;
    }

    // Clean up all nodes in the circular list without infinite looping
    ~LinkedList() {
        if (head == nullptr) {
            return;
        }

        // Break the circle so linear deletion stops cleanly
        tail->next = nullptr;

        Node<T>* temp = head;
        while (temp != nullptr) {
            Node<T>* nextNode = temp->next;
            delete temp;
            temp = nextNode;
        }

        head = nullptr;
        tail = nullptr;
        current = nullptr;
    }
};

int main() {
    LinkedList<Song> myPlaylist;

    Song* s1 = new Song("Nightcall", "Kavinsky", 258);
    Song* s2 = new Song("Starboy", "The Weeknd", 230);
    Song* s3 = new Song("Midnight City", "M83", 243);

    myPlaylist.insert(s1);
    myPlaylist.insert(s2);
    myPlaylist.insert(s3);

    myPlaylist.displayList();

    cout << "\n--- Continuous Playback (playNext) ---" << endl;
    for (int i = 0; i < 5; ++i) {
        myPlaylist.playNext();
    }

    // Clean up dynamically allocated Song objects
    delete s1;
    delete s2;
    delete s3;

    return 0;
}