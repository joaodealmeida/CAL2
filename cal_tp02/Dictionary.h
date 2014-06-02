//
//  Dictionary.h
//  cal_tp02
//
//  Created by Eduardo Almeida on 13/05/14.
//  Copyright (c) 2014 Bitten Apps. All rights reserved.
//

#ifndef __cal_tp02__Dictionary__
#define __cal_tp02__Dictionary__

#include <iostream>
#include <vector>

class Dictionary {
    
    Dictionary();
    
    std::vector<std::string> _dict;
    
public:
    
    Dictionary(std::string path);
    
    bool containsWord(std::string word);
    
    std::vector<std::string> getWords(int length = 0);
};

#endif /* defined(__cal_tp02__Dictionary__) */
