#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

// Function to load file into a string
string load_file_into_string(const string& filename) {
    ifstream file(filename);
    string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();
    return content;
}

// Function to load file into a vector of strings
vector<string> load_file_into_vector(const string& filename) {
    vector<string> words;
    ifstream file(filename);
    string word;
    while (file >> word) {
        words.push_back(word);
    }
    file.close();
    return words;
}

// to_lower: Convert string to lowercase
string to_lower(const string& str) {
    string result = str;
    for (int i = 0; i < result.length(); i++) {
        if (result[i] >= 'A' && result[i] <= 'Z') {
            result[i] = result[i] + 32; // Convert uppercase to lowercase
        }
    }
    return result;
}

// is_alphabetic: Check if character is alphabetic
bool is_alphabetic(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

// convert_book_into_sentences: Convert book string into vector of sentences
vector<string> convert_book_into_sentences(const string& book) {
    vector<string> sentences;
    string current_sentence = "";
    
    for (int i = 0; i < book.length(); i++) {
        char c = book[i];
        current_sentence += c;
        
        // Check for sentence terminators
        if (c == '.' || c == '?' || c == '!') {
            // Remove leading/trailing whitespace
            int start = 0;
            int end = current_sentence.length() - 2; // Exclude the terminator
            
            while (start <= end && (current_sentence[start] == ' ' || current_sentence[start] == '\n' || current_sentence[start] == '\t')) {
                start++;
            }
            while (end >= start && (current_sentence[end] == ' ' || current_sentence[end] == '\n' || current_sentence[end] == '\t')) {
                end--;
            }
            
            if (start <= end) {
                string trimmed = current_sentence.substr(start, end - start + 1);
                sentences.push_back(trimmed);
            }
            
            current_sentence = "";
        }
    }
    
    return sentences;
}

// convert_sentences_into_words: Convert sentence string into vector of words
vector<string> convert_sentences_into_words(const string& sentence) {
    vector<string> words;
    string current_word = "";
    
    for (int i = 0; i < sentence.length(); i++) {
        char c = sentence[i];
        
        if (is_alphabetic(c) || c == '\'') { // Include apostrophes in words
            current_word += c;
        } else if (c == ' ' || c == '\t' || c == '\n') {
            if (current_word.length() > 0) {
                words.push_back(current_word);
                current_word = "";
            }
        } else {
            // Handle other symbols (punctuation)
            if (current_word.length() > 0) {
                words.push_back(current_word);
                current_word = "";
            }
        }
    }
    
    // Add the last word if any
    if (current_word.length() > 0) {
        words.push_back(current_word);
    }
    
    return words;
}

// is_string_in_vector: Check if string is in vector
bool is_string_in_vector(const string& str, const vector<string>& vec) {
    for (int i = 0; i < vec.size(); i++) {
        if (to_lower(vec[i]) == to_lower(str)) {
            return true;
        }
    }
    return false;
}

// sentence_type structure
struct sentence_type {
    vector<string> words;
    
    // Default constructor
    sentence_type() {
        words.clear();
    }
    
    // Constructor that takes a sentence string
    sentence_type(const string& sentence) {
        words = convert_sentences_into_words(sentence);
    }
    
    // Print function
    void print() {
        for (int i = 0; i < words.size(); i++) {
            cout << words[i];
            if (i < words.size() - 1) {
                cout << " ";
            }
        }
        cout << "." << endl;
    }
};

// replace_adverbs: Replace adverbs in sentence with random adverbs
sentence_type replace_adverbs(const sentence_type& original_sentence, const vector<string>& adverb_list) {
    sentence_type modified_sentence;
    modified_sentence.words = original_sentence.words;
    
    for (int i = 0; i < modified_sentence.words.size(); i++) {
        if (is_string_in_vector(modified_sentence.words[i], adverb_list)) {
            // Replace with random adverb
            int random_index = rand() % adverb_list.size();
            modified_sentence.words[i] = adverb_list[random_index];
        }
    }
    
    return modified_sentence;
}

// Main program
int main() {
    srand(time(0)); // Seed random number generator
    
    cout << "Loading book file..." << endl;
    string book = load_file_into_string("book.txt");
    
    cout << "Converting book into sentences..." << endl;
    vector<string> sentences = convert_book_into_sentences(book);
    
    cout << "Loading adverb list..." << endl;
    vector<string> adverbs = load_file_into_vector("common_adverbs.txt");
    
    // Convert adverbs to lowercase
    cout << "Converting adverbs to lowercase..." << endl;
    for (int i = 0; i < adverbs.size(); i++) {
        adverbs[i] = to_lower(adverbs[i]);
    }
    
    cout << "\n=== ADVERB REPLACEMENT EXPERIMENT ===" << endl;
    cout << "Processing first 10 sentences..." << endl << endl;
    
    // Process first 10 sentences
    int num_sentences = (sentences.size() < 10) ? sentences.size() : 10;
    
    for (int i = 0; i < num_sentences; i++) {
        cout << "--- Sentence " << (i + 1) << " ---" << endl;
        
        // Original sentence
        sentence_type original(sentences[i]);
        cout << "Original: ";
        original.print();
        
        // Modified sentence
        sentence_type modified = replace_adverbs(original, adverbs);
        cout << "Modified: ";
        modified.print();
        
        cout << endl;
    }
    
    return 0;
}
