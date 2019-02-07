/*
 *
 * Author: Jay Patel
 *
 *
 * Welcome to the world of Steiner Trees.
 *
 * This program lets you add the points and lines where ever you want and gives you the,
 * length of your line in total, to see wether you found the most efficient way to join,
 * all the dots or not.
 *
 * So, lets get Started...
 */

#include <iostream>    //input and output library file
#include "gobjects.h"  // for all graphics objects
#include "gevents.h"   // for mouse events
#include <fstream>     // For file input
#include <cmath>       // For all the math functions.
#include <ctime>       // time library

using namespace std;

// Global constants
const int PointRadius = 7;  //Point radius will be constant during the whole program.
GWindow graphicsWindow;     //Global Graphics window so that we can access it anywhere.

//Class definitions for creating the points and the buttons on the graphic window.

// Class to make buttons on the screen.
class MyButton
{
public:
    //Constructer to assign values to the private variables.
    MyButton(int x, int y, int width, int height, string color,string message)
    {
        xValue=x;
        yValue=y;
        Color=color;
        rHeight=height;
        rWidth=width;
        Labal=message;
    }//end of Constructor.

    //Function to make the rectangles for the Buttons.
    void makeRectangle(GRect *MyRectangle,GLabel *Label)
    {
        MyRectangle=new GRect(xValue,yValue,rWidth,rHeight);
        MyRectangle->setColor(Color);
        MyRectangle->setFilled(true);
        graphicsWindow.add(MyRectangle);

        //To print out the labels on top of the Buttons.
        Label = new GLabel();
        Label->setText(Labal);

        //Giving the Bounds to the lables.
        int xOffset1 = (rWidth  - Label->getBounds().getWidth()) / 2;
        int yOffset1 = (rHeight - Label->getBounds().getHeight()) + 5;
        Label->setLocation( xValue + xOffset1, yValue + yOffset1);
        graphicsWindow.add(Label);
    }//end of the Make Rectangle function.

private:
    //Private variable declaration.
    int xValue;
    int yValue;
    int rHeight;
    int rWidth;
    string Color;
    string Labal;
    GRect *MyRectangle;
};//End of MyButton Class.
//------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------
//Class to creat points on the graphics window.
class MyPoints
{
public:
    //Function to set the values to variables.
    setPoint(int X, int Y, string Color)
         {
             x=X;
             y=Y;
             dotColor= Color;
         }
    getPointX()
         {
             return x;
         }
    getPointY()
         {
             return y;
         }

    //Function to make the points on the screen.
    void makePoint(int x, int y,string Color)
    {
        dotColor=Color;
        // Center the oval to be drawn on the x,y location. PointRadius is a global constant.
        GOval *pTheOval = new GOval( x-PointRadius, y-PointRadius, PointRadius*2, PointRadius*2);
        pTheOval->setFilled( true);
        pTheOval->setColor( dotColor);
        graphicsWindow.add( pTheOval);
    }//End of the Function.

private:
    //Private varible declarations.
    string dotColor;
    int x;
    int y;
}; //End of the MyPoints Class.
//-----------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------
//Function with the Object Datatype to return the position of the points from the screen.
//So that we can set the start and the end of the line to a nearest point.
MyPoints nearestPoint(Vector<MyPoints> &pointsArray,int mouseX,int mouseY,int numPoint)
{
    //Variables to compare and assign the colsest distance.
    int len[numPoint];
    int closeDist;

    //For loop to check all the points, and their distances from the dragged line coordinates.
    for(int i=0;i<numPoint;i++)
    {
        len[i]=sqrt((pointsArray[i].getPointX()-mouseX)*(pointsArray[i].getPointX()-mouseX)
                  +(pointsArray[i].getPointY()-mouseY)*(pointsArray[i].getPointY()-mouseY));
        closeDist=len[i];
    }//End of for loop.

    //For loop to check which one is closest.
    for(int i=0;i<numPoint;i++)
    {
        if(len[i] <closeDist)
        {
            closeDist= len[i];
        }
    }//end of for loop.

    //For loop to return the point.
    for(int i=0;i<numPoint;i++)
    {
        if(closeDist==len[i])
        {
           return pointsArray[i];
        }
    }//end of for loop
}//End of the function.
//---------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------
//Function to read into the data.txt file.
int readInputFile( Vector<MyPoints> &pointsArray, int &idealScore)
{
    ifstream inputFileStream;  // declare the input file stream

    inputFileStream.open("data.txt");
    if( !inputFileStream.is_open())
    {
        cout << "Could not find input file data.txt.  Exiting..." << endl;
        exit( -1);
    }

    // Read sets of data from input file.  Each read operation reads the next set of data.
    // The first line in the input file indicates which set to use.
    int whichSetToUse;
    inputFileStream >> whichSetToUse;

    int howManyPoints;    // How many points are in the set of points
    int x, y;             // Store individual point x,y values from input file
    // Reserve some reasonable number of total points for the vector, so that
    pointsArray.ensureCapacity( 10);

    // Read in and discard some number of sets of points before we get the real set we will be using
    for( int i = 0; i < whichSetToUse-1; i++)
    {
        inputFileStream >> howManyPoints >> idealScore;
        for( int j = 0; j < howManyPoints; j++)
        {
            inputFileStream >> x >> y;    // Read in the points
        }
    }

    inputFileStream >> howManyPoints >> idealScore;

    // Read in and store the points information
    for( int i = 0; i < howManyPoints; i++)
    {
       inputFileStream >> x >> y;    // Read in the points

       //setting and making the points from the file.
       pointsArray[i]. setPoint(x,y,"black");
       pointsArray[i]. makePoint(x,y,"black");

       cout << "Point " << i << " is at " << x << "," << y << endl;
    }//End of for loop.
    return howManyPoints;
}//end readInputFile()
//------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------
//Main Function of the program, this where the logic turns into magic.
int main()
{
    graphicsWindow.setCanvasSize( 400, 400);   // Set the size of graphic window.
    graphicsWindow.setWindowTitle("jpate225  QtCreator Program 6");

    Vector< MyPoints> pointsArray(200);       // Store all points that are created.

    int idealScore = 0;                       // Precomputed ideal score for each puzzle

    int totalPoints=0;                        //To keep track of how many points user draws.

    int lenLine=0;                            //To keep teck of the length

    MyPoints close;                           //Creating the object to call the class function.

    // Get the set of data to use from the input file
    totalPoints=readInputFile( pointsArray, idealScore);

    //giving idealScore the value of 105%.
    idealScore=idealScore+((idealScore*5)/100);

    //Creating the labal to print out on a initial window.
    GLabel *messagesLabel = new GLabel();
    messagesLabel->setText("Find the minimal spanning tree, using additional points.");
    messagesLabel->setLocation( 30,20);
    graphicsWindow.add( messagesLabel);

    // Exit button enclosing red rectangle variables.
    int exitButtonX = 250;
    int exitButtonY = 350;
    int exitButtonWidth = 50;
    int exitButtonHeight = 30;

    // Add Points Button enclosing green rectangle variables.
    int addPointButtonX=10;
    int addPointButtonY=350;
    int addPointButtonWidth=70;
    int addPointButtonHeight=30;

    // Draw lines Button enclosing the gray button variables.
    int drawLinesButtonX=90;
    int drawLinesButtonY=350;
    int drawLinesButtonWidth=70;
    int drawLinesButtonHeight=30;

    //creating button objects for the class, and initalizing the values for buttons.
    MyButton exitButtonObj(exitButtonX, exitButtonY, exitButtonWidth, exitButtonHeight,"red","Exit");
    MyButton addPointButtonObj(addPointButtonX, addPointButtonY, addPointButtonWidth, addPointButtonHeight,"green","Add Points");
    MyButton drawLinesButtonObj(drawLinesButtonX,drawLinesButtonY,drawLinesButtonWidth,drawLinesButtonHeight,"gray","Draw Lines");

    //Declaring the rectangles with GRect label.
    GRect *exitButton=new GRect(exitButtonX, exitButtonY, exitButtonWidth, exitButtonHeight);
    GLabel *exit;
    GRect *addPointButton = new GRect(addPointButtonX, addPointButtonY, addPointButtonWidth, addPointButtonHeight);
    GLabel *addPoint;
    GRect *drawLinesButton = new GRect(drawLinesButtonX,drawLinesButtonY,drawLinesButtonWidth,drawLinesButtonHeight);
    GLabel *drawLines;

    //Finally Making the Rectangles.
    exitButtonObj.makeRectangle(exitButton,exit);
    addPointButtonObj.makeRectangle(addPointButton,addPoint);
    drawLinesButtonObj.makeRectangle(drawLinesButton,drawLines);

    //Make the graphics window have focus, since all interaction will be through the window
    graphicsWindow.requestFocus();

    // Set values to use for keeping track of program modes
    // Modes are Default, DrawPoints, DrawLines. Start in Default mode
    // Change to the other modes when those buttons are pressed.

    const int Default = 0;
    const int DrawPoints = 1;
    const int DrawLines = 2;
    int programMode = Default;

    // Set to false when a button is pressed.  Used to avoid drawing on buttons
    // immediately when they are pressed.
    bool buttonWasPressed = false;

     // Will store mouse position
    int mouseX = -1;
    int mouseY = -1;

    // Used to construct message to be displayed at top of window
    char message[ 81];

    // Used to Construct the message of length.
    char length2[100];
    GLine *pLine;

    //To Print out the Label.
    GLabel *lenLabel = new GLabel();
    lenLabel->setText(" Length ");
    lenLabel->setLocation( 320,350);
    graphicsWindow.add( lenLabel);

    //To print out the updating value of Length.
    GLabel *lenLabel2 = new GLabel();
    lenLabel2->setText("0");
    lenLabel2->setLocation( 330,370);
    graphicsWindow.add( lenLabel2);


    GMouseEvent e;      // Stores the mouse event each time through the event loop
    int counter = 0;    // Used to display debugging information, to help understand how the event loop works.

    int tempLine=0;     //This is find the length of each line.
    int temp=0;         //variable to store the updated value of the lenLine each time.
    int lineCount=1;    //to count how many lines user draws.

    //Starting of the main While Loop.
    while (true)
    {
        // Wait for a mouse event, then get the x,y coordinates
        e = waitForEvent(MOUSE_EVENT);

        // Get the current mouse x,y location
        mouseX = e.getX();
        mouseY = e.getY();

        counter++;    // Iterations counter

        //  -------- Perform actions for different kinds of mouse events --------
        if (e.getEventType() == MOUSE_PRESSED)
        {
            sprintf( message,"Mouse pressed at %d,%d", mouseX, mouseY);  // print into a string
            messagesLabel->setLabel(message);

            //When the Program mode is in DrawPoints.
            if(programMode==DrawPoints)
            {
                if(buttonWasPressed==true)
                {
                    //Condition to stop printing things on top of buttons.
                    if((addPointButton->contains(mouseX,mouseY)==false)
                        && (drawLinesButton->contains(mouseX,mouseY)==false)
                        && (exitButton->contains(mouseX,mouseY)==false))
                    {
                        pointsArray[totalPoints].setPoint(mouseX,mouseY,"blue");
                        pointsArray[totalPoints] .makePoint(mouseX,mouseY,"blue");
                        totalPoints++;
                    }//end of if..
                }//end of if..
            }//end of if..

            //Condition to see whether user pressed on right button or not.
            if(addPointButton->contains(mouseX,mouseY)==true)
            {
                buttonWasPressed=true;
                programMode=DrawPoints;
            }// end of if..


            // See if Exit button was clicked by seeing if mouse press location is inside the rectangle
            if( exitButton->contains( mouseX, mouseY) )
            {
                messagesLabel->setLabel("Exiting...");
                break;
            }

            //Condition to see whether user clicked on the right button or not.
            if(drawLinesButton->contains(mouseX,mouseY))
            {
                buttonWasPressed=true;
                programMode=DrawLines;
            }

            //Changing the Program Mode into DrawLines mode.
            if(programMode==DrawLines)
            {
                cout << "Setting line start and end points to " << mouseX << "," << mouseY << endl;
                messagesLabel->setLabel(message);

                if((addPointButton->contains(mouseX,mouseY)==false)
                   && (drawLinesButton->contains(mouseX,mouseY)==false)
                   && (exitButton->contains(mouseX,mouseY)==false))
                {
                    //Creating new line each time user wants to create one.
                    pLine = new GLine(0,0,0,0);
                    pLine->setLineWidth(2);
                    graphicsWindow.add(pLine);

                    //Giving the colosest point's starting points to the line start points.
                    close=nearestPoint(pointsArray, mouseX,mouseY,totalPoints);
                    pLine->setStartPoint(close.getPointX(),close.getPointY());//mouseX, mouseY);
                    pLine->setEndPoint( mouseX, mouseY);
                    lineCount++;
                }//end of if...
            }//end of if...
        }//END of Mouse Pressed if Condition....
        //-----------------------------------------------------------------------------------------------

        //-----------------------------------------------------------------------------------------------
        //Mouse Released Event else if Condition.
        else if( e.getEventType() == MOUSE_RELEASED)
        {
            cout << "Mouse was released at : " << mouseX << "," << mouseY << endl;

            if(programMode==DrawLines)
            {
                if((addPointButton->contains(mouseX,mouseY)==false)
                    && (drawLinesButton->contains(mouseX,mouseY)==false)
                    && (exitButton->contains(mouseX,mouseY)==false))
                {
                    //Again setting up the closest end points.
                    close=nearestPoint(pointsArray, mouseX,mouseY,totalPoints);
                    pLine->setEndPoint(close.getPointX(),close.getPointY());

                    //Using Pythagorus Theoram to find the length of the line.
                    tempLine=sqrt(((pLine->getHeight())*(pLine->getHeight()))
                                  +((pLine->getWidth())*(pLine->getWidth())));

                    //Updating the Length of Line.
                    lenLine=lenLine+tempLine;

                    //Storing the Updated lenLine Value into Temp Variable.
                    temp=lenLine;

                    //Using Sprintf to add into strings.
                    sprintf( length2,"%d",lenLine);
                    lenLabel2->setLabel(length2);
                }//End of if....
                //Condition for the messages to be displaed if you win or loose.
                if(lenLine<=idealScore && lenLine!=0)
                {
                    sprintf(message,"YES! %d is less or close to goal of %d",lenLine,idealScore);
                    messagesLabel->setLabel(message);

                }//end of if...
                if(lenLine>idealScore && lenLine!=0)
                {
                    sprintf(message,"Sorry! %d is a lot longer then %d",lenLine,idealScore);
                    messagesLabel->setLabel(message);
                }//end of if...

            }//end of DrawLine mode if condition.
        }//End of the else if condtion for mouse released.
        //----------------------------------------------------------------------------------------------------

        //----------------------------------------------------------------------------------------------------
        //else if condition for when mouse is being Dragged..
        else if(e.getEventType() == MOUSE_DRAGGED)
        {
            cout << "Mouse dragged to " << mouseX << "," << mouseY << endl;

            if(programMode==DrawLines)
            {
                if((addPointButton->contains(mouseX,mouseY)==false)
                   && (drawLinesButton->contains(mouseX,mouseY)==false)
                   && (exitButton->contains(mouseX,mouseY)==false))
                {
                    tempLine=sqrt(((pLine->getHeight())*(pLine->getHeight()))
                                    +((pLine->getWidth())*(pLine->getWidth())));

                    sprintf( length2," %d",temp+tempLine);
                    lenLabel2->setLabel(length2);
                    pLine->setEndPoint(mouseX, mouseY);
                }//end of if condition
            }//end of DrawLine condition...
        }//End of the else if condition for when the MOUSE is DRAGGED...
        //---------------------------------------------------------------------------------------------

        //---------------------------------------------------------------------------------------------
    }//end while( true) loop here

    // Closeing the windows, first the graphics window
    graphicsWindow.requestFocus();
    graphicsWindow.close();
    exitGraphics();

    return 0;
}//The Main Function as well as the Program Ends here.
//-------------------------------------------------------------------------------------------------------
//                                                THE END
//-------------------------------------------------------------------------------------------------------
