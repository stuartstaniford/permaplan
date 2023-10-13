// Copyright Staniford Systems.  All Rights Reserved.  September 2023 -

// Javascript code for making graphs in Permaplan/Permaserv using D3.


// =======================================================================================
/// @brief class for keeping track of svg/D3 based ScatterPlots in progress.
///
/// Used so we can incrementally add data as it is fetched from the web.

class SvgScatterPlot 
{

  // =======================================================================================
  /// @brief Constructor for an SvgScatterPlot.
  /// 
  /// @param svgIdName The id of the svg that the graph should be drawn in.
  /// @param xLo The low end of the initial x range for the plot
  /// @param xHi The high end of the initial x range for the plot
  /// @param yLo The low end of the initial y range for the plot
  /// @param yHi The high end of the initial y range for the plot

  constructor(svgIdName, xLo, xHi, yLo, yHi) 
   {
    // Find our SVG where we will be drawing the scatterplot
    this.svg = d3.select("#" + svgIdName);
    if(!this.svg.node()) 
     {
      console.error("Cannot find svg area " + svgIdName + " in page.");
      alert("Cannot find svg area " + svgIdName + " in page.");
     }

    // Store our parameters for future use.
    this.xLow   = xLo;
    this.xHigh  = xHi;
    this.yLow   = yLo;
    this.yHigh  = yHi;
    
    // Initialize the array for storing the names of series
    this.seriesNames      = [];
    this.lastData         = [];
    this.dataPointsAdded  = 0;
    this.data             = {};
    
    // Set margins and find the dimensions of the svg
    this.margin = { top: 20, right: 20, bottom: 30, left: 40 };
    this.width = +this.svg.attr("width") - this.margin.left - this.margin.right;
    this.height = +this.svg.attr("height") - this.margin.top - this.margin.bottom;

    // Set the computed domains to the scales
    this.xScale = d3.scaleLinear().domain([this.xLow, this.xHigh]).range([0, this.width]);
    this.yScale = d3.scaleLinear().domain([this.yLow, this.yHigh]).range([this.height, 0]);

    // Setup the axes
    this.xAxis = d3.axisBottom(this.xScale);
    this.yAxis = d3.axisLeft(this.yScale);

    // Draw the graph within the svg's g element
    this.graphGroup = this.svg.append("g")
                            .attr("transform", `translate(${margin.left},${margin.top})`);

    // Make a place to put the data representation later
    this.dataGroup = this.graphGroup.append("g");

    // Draw the x and y axis
    this.graphGroup.append("g")
      .attr("transform", `translate(0,${height})`)
      .call(this.xAxis);
    this.graphGroup.append("g")
      .call(this.yAxis);

    // Create the line generator
    this.lineGenerator = d3.line()
      .x(d => xScale(d.x))
      .y(d => yScale(d.y));
    
    // End of constructor which doesn't return anything.
   }

  
  // =======================================================================================
  /// @brief Adding some extra series onto the end of the ones we were already
  /// tracking.
  /// 
  /// @param addedNames An array of the new names of series to be added.  Note, they must
  /// all be unique.  The first one is ignored (because it's above the x-values, not the
  /// series of y-values).

  addSeriesNames(addedNames) 
   {
    const oldLength = this.seriesNames.length;
    this.seriesNames.push(...addedNames);
    for(let i=oldLength; i<this.seriesNames.length; i++)
     {
      this.lastData[i] = NaN;
      this.data[this.seriesNames[i]] = [];
     }
   }
  
  
  // =======================================================================================
  /// @brief Add some circles to represent one row of data to scatter plot.
  /// 
  /// @param points The data to be plotted
  /// NB the structure of the data object of the series and points to plot is as follows.  
  /// Example data with two series:
  /// const points = [
  ///  { "A": [{ x: 1, y: 5 }, { x: 2, y: 7 }, { x: 3, y: 10 }],},
  ///  { "B": [{ x: 1, y: 10 }, { x: 2, y: 15 }, { x: 3, y: 17 }] }


  addCircles(points) 
   {
    if (!points || typeof points !== 'object') 
     {
      console.error("Invalid points data passed to addCircles.");
      return;
     }

    for (let series of Object.keys(points))
     {
      this.dots = this.dataGroup.selectAll(".dot")
                      .data(points[series]);
      this.dots.enter().append("circle")
        .attr("cx", d => this.xScale(d.x))
        .attr("cy", d => this.yScale(d.y))
        .attr("r", 5)
        .attr("fill", "steelblue");
     }
   }


  // =======================================================================================
  /// @brief Process one incoming line from the url with the (tab delimited) header or data.
  /// 
  /// @param line The (unprocessed) string we got from one line of the data file
  /// NB the structure of the data object of the series and points to plot is as follows.  
  /// Example data with two series:
  /// const points = [
  ///  { "A": [{ x: 1, y: 5 }, { x: 2, y: 7 }, { x: 3, y: 10 }],},
  ///  { "B": [{ x: 1, y: 10 }, { x: 2, y: 15 }, { x: 3, y: 17 }] }

  processLine(line) 
   {
    if(this.seriesNames.length)
     {
      // We already have series header, so treat the line as data
      const terms = line.trim().split('\t');
      
      for(let i=1; i<points.length; i++)
       {
        this.data[this.seriesNames[i]].push({x: terms[0] , y: terms[i]});
       }

      // Plot the circles
      this.addCircles(this.data);
      //this.updateLastData(this.data);
      
      // If these are not the first data points, add lines from the last ones
      if(this.dataPointsAdded)
       {
        //this.plotLines(this.data);
       }
      this.dataPointsAdded++;
     }
    else
     {
      // Assume the first line is the header names
      addSeriesNames(line.trim().split('\t'));
     }
   }
  
  
// =======================================================================================
/// End of class SvgScatterPlot 

}


// =======================================================================================
/// @brief Clears the SVG to avoid appending multiple elements.
///
/// @param svgIdName The id of the svg to put this scatterplot.

function clearSVG(svgIdName) 
{
  d3.select("#" + svgIdName).selectAll("*").remove();
}


// =======================================================================================
/// @brief Gets the station-id when the Url is of the form
/// http://blah-host:blah-port/blah-path/USC00304174
/// 
/// @returns The station-id

function getStationIdFromCurrentPath()
{
  const pathComponents = window.location.pathname.split('/');
  return pathComponents[pathComponents.length - 1];
}


// =======================================================================================
/// @brief Compute scatterplot scales from the data after we have gotten it.
/// 
/// @param svgIdName The id of the svg to put this scatterplot.
/// @param observable One of minTempStation, maxTempStation, precipStation

function computeScaleDomains(data) 
{
  let xMin = Infinity, xMax = -Infinity, yMin = Infinity, yMax = -Infinity;

  data.forEach((series) => 
   {
    series.values.forEach((point) => 
     {
      xMin = Math.min(xMin, point.x);
      xMax = Math.max(xMax, point.x);
      yMin = Math.min(yMin, point.y);
      yMax = Math.max(yMax, point.y);
     });
   });

  return {
    xDomain: [xMin, xMax],
    yDomain: [yMin, yMax]
  };
}


// =======================================================================================
/// @brief Fetch data from a url and process it in a line oriented way as it arrives.
///
/// @param url The url from whence to get the data
/// @param objectArg An object must have a processLine function, which takes one line and 
/// does something with it.

async function fetchAndProcessStream(url, objectArg) 
{
  const textDecoder = new TextDecoder('utf-8');
  let lastPartialLine = '';

  try
   {
    const response = await fetch(url);
    const reader = response.body.getReader();

    while (true) 
     {
      // Get a chunk of the response body (eg one packet's worth) and decode it
      const { done, value } = await reader.read();
      if (done) 
       {
        break;
       }
      let textChunk = textDecoder.decode(value, { stream: true });
      
      let lines = textChunk.split('\n');
      lines[0] = lastPartialLine + lines[0];
      lastPartialLine = lines.pop(); // save partial line for the next chunk
          
      for (let line of lines) 
       {
        objectArg.processLine(line);
       }
     }

    if (lastPartialLine) 
     {
      objectArg.processLine(lastPartialLine);
     }
   } 
  catch (error) 
   {
    console.error(`Error fetching and processing stream: ${error}`);
   }
}


// =======================================================================================
/// @brief Gets a particular observable from permaserv and scatterplots it
/// 
/// @param svgIdName The id of the svg to put this scatterplot.
/// @param observable One of minTempStation, maxTempStation, precipStation
/// @param xLo The low end of the x range for the plot
/// @param xHi The high end of the x range for the plot
/// @param yLo The low end of the y range for the plot
/// @param yHi The high end of the y range for the plot

function scatterPlotObservable(svgIdName, observable, xLo, xHi, yLo, yHi)
{
  const urlStub = '/climate/';
  const stationId = getStationIdFromCurrentPath();
  const url = urlStub + observable + "/" + stationId;
  const svgPlot = new SvgScatterPlot(svgIdName, xLo, xHi, yLo, yHi);

  fetchAndProcessStream(url, svgPlot)
      .catch(
            (error) => {
              console.error(`Error occurred fetching data from ${url}: ${error}`);
              alert(`Error occurred fetching data from ${url}: ${error}`);
            });
}


// =======================================================================================


document.addEventListener("DOMContentLoaded", function() {
  scatterPlotObservable("maxtemp", "maxTempStation", 0, 365, 0, 110);
  scatterPlotObservable("mintemp", "minTempStation", 0, 365, -20, 90);
});


// =======================================================================================
