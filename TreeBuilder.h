
#ifndef _TREE_BUILDER_H_
#define _TREE_BUILDER_H_

#include "BasicDataStructures.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


using namespace BasicDataStructures;

class TreeBuilder
{
public:
    TreeBuilder();
    ~TreeBuilder();

    void BuildTree( const vector<Item>& iv, const vector<NumericAttr>& fv, const vector<char*>& cv );
    void SetGiniSplitThreshold( float gst );
    TreeNode* GetRoot();

private:
    TreeNode* Split( const vector<Item>& iv, unsigned short* featureIndexArray, unsigned int featureIndexArraySize, int height );
    float ComputeGini( const vector<Item>& iv );
    void DestroyNode( TreeNode* node );

    vector<char*> classVec;
    vector<NumericAttr> featureVec;
    vector<Item> itemVec;

    TreeNode* root = nullptr;
    
    // Settings
    float giniSplitThreshold;
};

#endif
