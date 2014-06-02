//
//  WordMind.cpp
//  cal_tp02
//
//  Created by Eduardo Almeida on 13/05/14.
//  Copyright (c) 2014 Bitten Apps. All rights reserved.
//

#include <map>

#include "WordMind.h"

WordMind::WordMind(Dictionary *dict, unsigned long length) {
    _possibleWords = dict -> getWords((int) length);
}

std::vector<WordMindState> WordMind::generateState(std::string correctWord, std::string guessedWord) {
    std::vector<WordMindState> stateVector;
    
    if (correctWord.length() != guessedWord.length())
        throw "Impossible Comparision!";
    
    std::map<char, int> wordMap;
    
    for (auto i = 0; i < correctWord.length(); i++) {
        auto it = wordMap.find(correctWord[i]);
        
        if (it != wordMap.end())
            (it -> second)++;
        else
            wordMap[correctWord[i]] = 1;
    }
    
    //  Initialize the State Vector
    
    for (auto i = 0; i < correctWord.length(); i++)
        stateVector.push_back(WordMindState::UNPROCESSED);
    
    //  Check for Exact Matches
    
    for (auto i = 0; i < correctWord.length(); i++)
        if (correctWord[i] == guessedWord[i]) {
            stateVector[i] = WordMindState::CORRECT;
            wordMap[correctWord[i]]--;
        }
    
    //  Check for Out of Place / Not Found
    
    for (auto i = 0; i < correctWord.length(); i++) {
        if (stateVector[i] != WordMindState::UNPROCESSED)
            continue;
        
        auto it = wordMap.find(guessedWord[i]);
        
        if (it != wordMap.end() && wordMap[guessedWord[i]]) {
            stateVector[i] = WordMindState::WRONG_PLACE;
            wordMap[guessedWord[i]]--;
        } else
            stateVector[i] = WordMindState::NOPE;
    }
    
    return stateVector;
}

void WordMind::_parseGuessState() {
    std::vector<char> mustNotContain;
    
    std::map<int, char> mustContainAt;
    std::map<int, char> mustContainButNotAt;
    std::map<char, int> mustContainExactly;
    
    for (auto i = 0; i < _currentWord.length(); i++) {
        switch (_guessState[i]) {
            case WordMindState::NOPE:
                
                if (std::find(mustNotContain.begin(), mustNotContain.end(), _currentWord[i]) != mustNotContain.end())
                    break;
                
                mustNotContain.push_back(_currentWord[i]);
                
                break;
                
            case WordMindState::WRONG_PLACE:
                
                mustContainButNotAt.insert(std::pair<int, char>(i, _currentWord[i]));
                
                break;
                
            case WordMindState::CORRECT:
                
                mustContainAt.insert(std::pair<int, char>(i, _currentWord[i]));
                
                break;
                
            default:
                
                throw "Are you serious?!";
                
                break;
        }
    }
    
    for (auto c : mustContainAt) {
        auto f = std::find(mustNotContain.begin(), mustNotContain.end(), c.second);
        
        if (f != mustNotContain.end())
            mustNotContain.erase(f);
    }
    
    for (auto it = mustContainButNotAt.begin(); it != mustContainButNotAt.end(); it++) {
        auto c = it->second;
        
        if (mustContainExactly.find(c) != mustContainExactly.end())
            continue;
        
        auto notPos = std::find(mustNotContain.begin(), mustNotContain.end(), c);
        
        if (notPos != mustNotContain.end()) {
            //  We can now know EXACTLY how many characters "c" are in the word.
            
            unsigned int count = 0;
            
            for (auto it = mustContainButNotAt.begin(); it != mustContainButNotAt.end(); it++)
                if (it -> second == c)
                    count++;
            
            mustContainExactly.insert(std::pair<char, int>(c, count));
            
            mustNotContain.erase(notPos);
        }
    }
    
    std::vector<char> mncHolder;
    std::map<int, char> mcaHolder;
    std::map<int, char> mcbnaHolder;
    std::map<char, int> mceHolder;
    
    mncHolder = mustNotContain;
    
    std::copy(mustContainAt.begin(), mustContainAt.end(), std::inserter(mcaHolder, mcaHolder.end()));
    std::copy(mustContainButNotAt.begin(), mustContainButNotAt.end(), std::inserter(mcbnaHolder, mcbnaHolder.end()));
    std::copy(mustContainExactly.begin(), mustContainExactly.end(), std::inserter(mceHolder, mceHolder.end()));
    
    for (auto i = 0; i < _possibleWords.size(); i++) {
        auto word = _possibleWords[i];
        
        bool removeWord = false;
        
        unsigned int mcbnaOccurences = 0;    //  Must contain but not at occurence counter...
        
        for (auto j = 0; j < word.length(); j++) {
            auto c = word[j];
            
            //  mustNotContain check
            
            for (auto mnc : mustNotContain)
                if (c == mnc) {
                    removeWord = true;
                    
                    break;
                }
            
            if (removeWord)
                break;
            
            //  mustContainExactly check
            
            if (mustContainExactly.find(c) != mustContainExactly.end())
                (mustContainExactly.find(c)->second)--;
            
            //  mustContainAt check
            
            if (mustContainAt.find(j) != mustContainAt.end())
                if (mustContainAt[j] != c)
                    removeWord = true;
            
            if (removeWord)
                break;
            
            //  mustContainButNotAt check
            
            for (auto it = mustContainButNotAt.begin(); it != mustContainButNotAt.end(); it++) {
                auto cs = it->second;
                
                if (cs == c)
                    mcbnaOccurences++;
            }
            
            if (mustContainButNotAt.find(j) != mustContainButNotAt.end())
                if (mustContainButNotAt[j] == c)
                    removeWord = true;
            
            if (removeWord)
                break;
        }
        
        for (auto it = mustContainExactly.begin(); it != mustContainExactly.end(); it++)
            if (it->second != 0) {
                removeWord = true;
                
                break;
            }
                
        
        if (removeWord || mcbnaOccurences < mustContainButNotAt.size()) {
            _possibleWords.erase(_possibleWords.begin() + i);
            
            i--;
        }
        
        mustNotContain = std::vector<char>();
        mustContainAt = std::map<int, char>();
        mustContainButNotAt = std::map<int, char>();
        mustContainExactly = std::map<char, int>();
        
        mustNotContain = mncHolder;
        
        std::copy(mcaHolder.begin(), mcaHolder.end(), std::inserter(mustContainAt, mustContainAt.end()));
        std::copy(mcbnaHolder.begin(), mcbnaHolder.end(), std::inserter(mustContainButNotAt, mustContainButNotAt.end()));
        std::copy(mceHolder.begin(), mceHolder.end(), std::inserter(mustContainExactly, mustContainExactly.end()));
    }
}

void WordMind::guess() {
    if (_currentWord.length())
        _parseGuessState();
    
    _currentWord = _possibleWords[0];
    
    if (!_possibleWords.size())
        throw WordMindRuntimeFailureException();
    
    _possibleWords.erase(_possibleWords.begin());
}

void WordMind::setState(std::vector<WordMindState> guessState) {
    _guessState = guessState;
}

std::string WordMind::getCurrentWord() {
    return _currentWord;
}

double WordMind::getCertainity() {
    return 1.0f / _possibleWords.size();
}
