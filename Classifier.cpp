
#include "Classifier.h"


Classifier::Classifier()
{

}

Classifier::~Classifier()
{
    // Destory trees
    for (TreeNode* root : rootVec) treeBuilder.DestroyNode( root );
    rootVec.clear();
}


void Classifier::Train(
    const vector<Item>& iv, 
    const vector<NumericAttr>& fv, 
    const vector<char*>& cv )
{
    classVec = cv;
    featureVec = fv;

    // Randomly select features and build trees.
    unsigned int numFeatures = fv.size();

    printf( "Num features: %d\n", numFeatures );
    
    // Generate an ordered index container, and disorder it.
    unsigned int* randomIndices = 
        (unsigned int*) malloc( numFeatures * sizeof( unsigned int ) );
    for (unsigned int i = 0; i < numFeatures; i++) randomIndices[i] = i;
    randomizeArray( randomIndices, numFeatures, numFeatures );

    // Build a number of trees each of which having 10 features.
    // What if numFeatures is 51 ?
    unsigned int numTrees = numFeatures / NUM_FEATURES_PER_TREE;
    rootVec.reserve( numTrees );
    treeBuilder.Init( fv, cv, NUM_FEATURES_PER_TREE );

    for (unsigned int treeIndex = 0; treeIndex < numTrees; treeIndex++)
    {
        unsigned int* featureIndexArr = (unsigned int*) 
            malloc( NUM_FEATURES_PER_TREE * sizeof( unsigned int ) );
        memcpy( featureIndexArr, 
            randomIndices + treeIndex * NUM_FEATURES_PER_TREE, 
            NUM_FEATURES_PER_TREE * sizeof( unsigned int ) );

        // Build one tree
        treeBuilder.BuildTree( iv, featureIndexArr );
        rootVec.push_back( treeBuilder.GetRoot() );

        /*if (treeIndex < 2)
        {
            printf( "\nTree %d\n", treeIndex );
            treeBuilder.PrintTree( rootVec[treeIndex] );
            printf( "done\n" );
        }*/
    }

    free( randomIndices );
    randomIndices = nullptr;
}

void Classifier::Classify( const vector<Item>& iv )
{
    if (classVec.empty())
    {
        printf( "Please train the model first.\n" );
        return;
    }

    unsigned int correctCounter = 0;
    unsigned int totalNumber = iv.size();

    for (const Item& item : iv)
        if (Classify( item ) == item.classIndex) correctCounter++;

    float correctRate = (float) correctCounter / (float) totalNumber;
    float incorrectRate = 1.0f - correctRate;

    printf( "Correct rate: %f\n", correctRate );
    printf( "Incorrect rate: %f\n", incorrectRate );
}

int Classifier::Classify( const Item& item )
{
    unsigned short numClasses = classVec.size();
    unsigned int* votes = (unsigned int*) 
        calloc( numClasses, sizeof( unsigned int ) );

    for (TreeNode* node : rootVec)
    {
        if (node == nullptr) continue;

        while (!node->childrenVec.empty())
        {
            unsigned int i = node->featureIndex;

            // If there are only 2 buckets, then classify them into 2 groups:
            // one group having feature value smaller than mean, 
            // another group having feature value greater than mean.
            if (featureVec[i].numBuckets == 2)
            {
                if (item.featureAttrArray[i] <= featureVec[i].mean)
                {
                    if (node->childrenVec[0] == nullptr)
                        break;
                    else
                        node = node->childrenVec[0];
                }
                else
                {
                    if (node->childrenVec[1] == nullptr)
                        break;
                    else
                        node = node->childrenVec[1];
                }
            }
            else
            {
                unsigned int bucketIndex = 
                    (item.featureAttrArray[i] - featureVec[i].min) / 
                    featureVec[i].bucketSize;
                
                if (bucketIndex >= featureVec[i].numBuckets)
                    bucketIndex = featureVec[i].numBuckets - 1;

                if (node->childrenVec[bucketIndex] == nullptr)
                    break;
                else
                    node = node->childrenVec[bucketIndex];
            }
        }

        votes[node->classIndex]++;
    }

    unsigned short classIndex = getIndexOfMax( votes, numClasses );

    free( votes );
    votes = nullptr;

    return classIndex;
}
