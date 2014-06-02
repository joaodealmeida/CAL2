//
//  Dictionary.cpp
//  cal_tp02
//
//  Created by Eduardo Almeida on 13/05/14.
//  Copyright (c) 2014 Bitten Apps. All rights reserved.
//

#include "Dictionary.h"

#include "Additions.h"

#include <algorithm>

Dictionary::Dictionary(std::string path) {
    auto dict = Additions::explode("\n", Additions::get_file_contents(path.c_str()));
    
    for (auto word : dict) {
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
        
        _dict.push_back(word);
    }
}

std::vector<std::string> Dictionary::getWords(int length) {
    if (!length)
        return _dict;
    
    std::vector<std::string> returnVector;
    
    for (auto word : _dict)
        if (word.length() == length)
            returnVector.push_back(word);
    
    return returnVector;
}

bool Dictionary::containsWord(std::string word) {
    for (auto w : _dict)
        if (w == word)
            return true;
    
    return false;
}
