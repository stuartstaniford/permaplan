// Copyright Staniford Systems.  All Rights Reserved.  September 2023 -

// Javascript code for making graphs in Permaplan/Permaserv using D3.

// =======================================================================================
/// @brief Make a scatter plot in a particular svg area of the page.
/// 
/// @param svgIdName The id of the svg that the graph should be drawn in.
/// @param data An array of the series and points to plot.  Example data with two series:
/// const data = [
///  { series: "A", values: [{ x: 1, y: 5 }, { x: 2, y: 7 }, { x: 3, y: 10 }] },
///  { series: "B", values: [{ x: 1, y: 10 }, { x: 2, y: 15 }, { x: 3, y: 17 }] }
/// ];

function scatterPlot(svgIdName, data) 
 {
  const svg = d3.select("#" + svgIdName);
  const margin = { top: 20, right: 20, bottom: 30, left: 40 };
  const width = +svg.attr("width") - margin.left - margin.right;
  const height = +svg.attr("height") - margin.top - margin.bottom;

  // Compute the scale domains dynamically
  const {xDomain, yDomain} = computeScaleDomains(data);

  console.log("xDomain for " + svgIdName + " is " + xDomain + ".\n");
  console.log("yDomain for " + svgIdName + " is " + yDomain + ".\n");
  
  // Set the computed domains to the scales
  const xScale = d3.scaleLinear().domain(xDomain).range([0, width]);
  const yScale = d3.scaleLinear().domain(yDomain).range([height, 0]);
  
  // Axis
  const xAxis = d3.axisBottom(xScale);
  const yAxis = d3.axisLeft(yScale);

  // Draw the graph within the svg's g element
  const g = svg.append("g")
    .attr("transform", `translate(${margin.left},${margin.top})`);

  // Draw the x and y axis
  g.append("g")
    .attr("transform", `translate(0,${height})`)
    .call(xAxis);

  g.append("g")
    .call(yAxis);

  // Create the line generator
  const lineGenerator = d3.line()
    .x(d => xScale(d.x))
    .y(d => yScale(d.y));

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
/// @brief Gets a particular observable from permaserv and scatterplots it
/// 
/// @param svgIdName The id of the svg to put this scatterplot.
/// @param observable One of minTempStation, maxTempStation, precipStation

function scatterPlotObservable(svgIdName, observable)
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
/// @brief Fetch data from a url and process it in a line oriented way as it arrives.
///
/// @param url The url from whence to get the data
/// @param processLine A function which takes one line and does something with it.
/// Example Usage
/// fetchAndProcessStream('your_url_here', line => {
///    console.log(line);  // or any other processing
/// });

async function fetchAndProcessStream(url, processLine) 
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
      processLine(line);
     }
   }

  if (lastPartialLine) 
   {
    processLine(lastPartialLine);
   }
}


// =======================================================================================


document.addEventListener("DOMContentLoaded", function() {
  scatterPlotObservable("maxtemp", "maxTempStation");
  scatterPlotObservable("mintemp", "minTempStation",);
});


// =======================================================================================
