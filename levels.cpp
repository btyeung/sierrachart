// The top of every source code file must include this line
#include "sierrachart.h"
#include <cmath>
#include <vector>
//#include <format>

// For reference, refer to this page:
// https://www.sierrachart.com/index.php?page=doc/AdvancedCustomStudyInterfaceAndLanguage.php

// This line is required. Change the text within the quote
// marks to what you want to name your group of custom studies. 
SCDLLName("Levels Export - Drawings")

//Helpers


//This is the basic framework of a study function. Change the name 'TemplateFunction' to what you require.
SCSFExport scsf_LevelsExport(SCStudyInterfaceRef sc)
{
	
	//TODO: bad location, needs to be moved
	/*
	SCString BuildLevelDict(float price_begin, float price_end = 0.0, SCString note = "")
		//TODO: return dict or JSON like structure to pass to Redis via HTTP (most likely)
	{
		SCString FinalOutput = "";
		sc.FormatString(FinalOutput, "{Start: %f End: %f Message: %s}", DrawingObject.BeginValue, DrawingObject.EndValue, DrawingObject.Text.GetChars());
		return FinalOutput;

	}*/
	
	
	// Set configuration variables
	if (sc.SetDefaults)

	{

		sc.GraphName = "Levels Outbound Sync";
		sc.GraphRegion = 0;
		sc.UpdateAlways = 1;

		// We have expressly set auto-looping to off because we are not filling
		// in any of the Subgraph[].Data[] arrays.  Since auto-looping will
		// cause this function to be called for every bar in the chart and all
		// we are doing is using tools to do drawings, it would be inefficient
		// to use auto-looping.
		sc.AutoLoop = 0;
		sc.Input[0].Name = "Line Color";
		sc.Input[0].SetColor(RGB(255, 0, 255));

		return;
	}

	//The following code will get all of the Ray drawings on the chart and make them the 'Line Color ' Input color, change the width to 10, and move the starting point back by one bar. Since this function is continuously called with sc.UpdateAlways, this continuously occurs so you will see the Ray drawings continuously move back.

	int DrawingIndex = 0;
	s_UseTool DrawingObject;

	sc.AddMessageToLog("Levels export started", 1);

	//format string example:
	//sc.FormatString(GSheetMessage, "Google sheet sync running... Index: %i, Recalc Interval: %i, Url: %s", sc.Index, RecalcIntervalMin, Url.GetChars());

	//TODO: build list of output values, each record needs symbol, price_start, price_end, notes, date (for starters)
	std::vector<SCString> Prices;

	//TODO: just get a list of prices first

	int TotalLevelCount = 0;

	while (sc.GetUserDrawnChartDrawing(sc.ChartNumber, DRAWING_HORIZONTALLINE, DrawingObject, DrawingIndex)) //For horizontal line
	{

		/*
		DrawingObject.Color = sc.Input[0].GetColor();
		DrawingObject.LineWidth = 10;
		int BarIndex = sc.GetNearestMatchForSCDateTime(sc.ChartNumber, DrawingObject.BeginDateTime.GetAsDouble());
		DrawingObject.BeginDateTime = sc.BaseDateTimeIn[BarIndex - 1];
		DrawingObject.AddMethod = UTAM_ADD_OR_ADJUST;
		sc.UseTool(DrawingObject);

		*/

		//output to log for now

		//get price and text
		SCString DrawingMessage = "";
		
		float Price = DrawingObject.BeginValue;

		SCString LevelEntry = "";
		sc.FormatString(LevelEntry, "{Start: %f End: %f Message: %s}", DrawingObject.BeginValue, DrawingObject.BeginValue, DrawingObject.Text.GetChars());
		Prices.push_back(LevelEntry);

		sc.FormatString(DrawingMessage, "Line (single) found %i, Price: %f Message: %s", DrawingIndex, Price, DrawingObject.Text.GetChars());
		sc.AddMessageToLog(DrawingMessage, 1);


		DrawingIndex++;
	}

	TotalLevelCount = DrawingIndex;

	DrawingIndex = 0;

	while (sc.GetUserDrawnChartDrawing(sc.ChartNumber, DRAWING_RECTANGLE_EXT_HIGHLIGHT, DrawingObject, DrawingIndex)) //For horizontal line
	{

		/*
		DrawingObject.Color = sc.Input[0].GetColor();
		DrawingObject.LineWidth = 10;
		int BarIndex = sc.GetNearestMatchForSCDateTime(sc.ChartNumber, DrawingObject.BeginDateTime.GetAsDouble());
		DrawingObject.BeginDateTime = sc.BaseDateTimeIn[BarIndex - 1];
		DrawingObject.AddMethod = UTAM_ADD_OR_ADJUST;
		sc.UseTool(DrawingObject);

		*/

		//output to log for now

		//get price and text
		//Prices.push_back(DrawingObject.BeginValue);

		SCString DrawingMessage = "";
		sc.FormatString(DrawingMessage, "Level (range) found %i, Start: %f End: %f Message: %s", DrawingIndex, DrawingObject.BeginValue, DrawingObject.EndValue, DrawingObject.Text.GetChars());
		sc.AddMessageToLog(DrawingMessage, 1);

		SCString LevelEntry = "";
		sc.FormatString(LevelEntry, "{Start: %f End: %f Message: %s}", DrawingObject.BeginValue, DrawingObject.EndValue, DrawingObject.Text.GetChars());
		Prices.push_back(LevelEntry);

		DrawingIndex++;
	}


	//output values
	SCString FinalMessage = "";
	sc.FormatString(FinalMessage, "Total Levels %i", TotalLevelCount, Prices.size());
	sc.AddMessageToLog(FinalMessage, 1);

	
	for (auto & price : Prices) {
		sc.AddMessageToLog(price, 1);
	}

	/*
	for (int i = 0; i < Prices.size(); ++i) {
		//std::cout << path[i] << ' ';
		//sc.AddMessageToLog(Prices[i], 1);
		sc.AddMessageToLog(Prices[i], 1);
	}*/

}
