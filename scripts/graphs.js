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
    this.seriesNames = [];
    
    // Set margins and find the dimensions of the svg
    this.margin = { top: 20, right: 20, bottom: 30, left: 40 };
    this.width = +this.svg.attr("width") - this.margin.left - this.margin.right;
    this.height = +this.svg.attr("height") - this.margin.top - this.margin.bottom;

    // Set the computed domains to the scales
    this.xScale = d3.scaleLinear().domain([this.xLow, this.xHigh]).range([0, width]);
    this.yScale = d3.scaleLinear().domain([this.yLow, this.yHigh]).range([height, 0]);

    // Setup the axes
    this.xAxis = d3.axisBottom(this.xScale);
    this.yAxis = d3.axisLeft(this.yScale);

    // Draw the graph within the svg's g element
    this.graphGroup = this.svg.append("g")
                            .attr("transform", `translate(${margin.left},${margin.top})`);

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
  /// @param addedNames An array of the new names of series to be added.

  addSeriesNames(addedNames) 
   {
    this.seriesNames.push(...addedNames);
   }
  
  
  // =======================================================================================
  /// @brief Add some data to update a scatter plot.
  /// 
  /// @param data An array of the series and points to plot.  Example data with two series:
  /// const data = [
  ///  { series: "A", values: [{ x: 1, y: 5 }, { x: 2, y: 7 }, { x: 3, y: 10 }] },
  ///  { series: "B", values: [{ x: 1, y: 10 }, { x: 2, y: 15 }, { x: 3, y: 17 }] }
  /// ];
  /// @todo Hasn't been rewritten to be in the object yet.
  addSeriesData(data) 
   {
    // Draw the lines and scatter points
    data.forEach(seriesData => {
      // Draw line
      g.append("path")
          .datum(seriesData.values)
          .attr("fill", "none")
          .attr("stroke", "steelblue")
          .attr("d", lineGenerator);

      // Draw scatter points
      g.selectAll(".dot")
          .data(seriesData.values)
          .enter().append("circle")
          .attr("cx", d => xScale(d.x))
          .attr("cy", d => yScale(d.y))
          .attr("r", 5)
          .attr("fill", "steelblue");
    });
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
/// @brief Fetches tab delimited data from a URL and processes it into an array.
/// 
/// The series are in columns, and the first column is the x values, and the subsequent
/// columns are the y-values for each case.
/// @returns The array of data
/// @param url The url from which to fetch the data.

async function fetchData(url) 
{
  // Fetch data from the URL
  const response = await fetch(url);
  const text = await response.text();

  // Split the text data by lines and then by tabs
  const lines = text.split('\n').map(line => line.trim().split('\t'));
  const headers = lines[0];  // Assuming the first row contains headers

  // Transpose the matrix to work with columns (series) instead of rows
  const transposed = headers.map((_, i) => lines.map(row => row[i]));

  const data = [];
  for (let i = 1; i < headers.length; i++) {  // Skip the first column, assuming it contains x-values
    const seriesName = headers[i];
    const seriesData = transposed[i].slice(1).map((y, j) => ({
      x: +transposed[0][j + 1],  // +1 to skip header
      y: +y
    }));
    console.log("Series name is " + seriesName + "\n");
    console.log("Series data is " + seriesData + "\n");
    data.push({
      series: seriesName,
      values: seriesData
    });
  }

  return data;
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
/// @param processLine A function which takes one line and does something with it.
/// @param objectArg An object which will be passed to the processLine function.
/// Example Usage
/// fetchAndProcessStream('your_url_here', (line, _) => {
///    console.log(line);  // or any other processing
/// }, {});

async function fetchAndProcessStream(url, processLine, objectArg) 
{
  const response = await fetch(url);
  const reader = response.body.getReader();
  const textDecoder = new TextDecoder('utf-8');
  let lastPartialLine = '';

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
      processLine(line, objectArg);
     }
   }

  if (lastPartialLine) 
   {
    processLine(lastPartialLine, objectArg);
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
  fetchData(url)
      .then(
            (data) => {
              clearSVG(svgIdName);
              scatterPlot(svgIdName, data);
            })
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
