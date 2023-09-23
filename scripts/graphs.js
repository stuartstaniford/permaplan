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
/// @returns The array of data
/// @param url The id of the svg that the graph should be drawn in.

async function fetchData(url) 
{
  // Fetch data from the URL
  const response = await fetch(url);
  const text = await response.text();

  // Split the text data by lines and then by tabs
  const lines = text.split('\n').map(line => line.split('\t'));
  const header = lines[0];
    
  const dataMap = new Map();

  // Process each line (skip the header)
  for (let i = 1; i < lines.length; i++) 
   {
    const [series, x, y] = lines[i];
        
    // If the series doesn't exist in our map, add it
    if (!dataMap.has(series)) 
     {
      dataMap.set(series, { series, values: [] });
     }

    // Push the x and y values to the series' values array
    dataMap.get(series).values.push({ x: +x, y: +y }); // '+' is used to convert string to number
    }

  return Array.from(dataMap.values());
}


// =======================================================================================
/// @brief Gets the station-id when the Url is of the form
/// http://blah-host:blah-port/blah-path/USC00304174
/// 
/// @returns The station-id

function getStationIdFromCurrentPath()
{
  var pathComponents = window.location.pathname.split('/');
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


document.addEventListener("DOMContentLoaded", function() {
  scatterPlotObservable("maxtemp", "maxTempStation");
  scatterPlotObservable("mintemp", "minTempStation",);
});


// =======================================================================================
