const svg = d3.select("svg");
const margin = { top: 20, right: 20, bottom: 30, left: 40 };
const width = +svg.attr("width") - margin.left - margin.right;
const height = +svg.attr("height") - margin.top - margin.bottom;

// Sample data: Two series
const data = [
    { series: "A", values: [{ x: 1, y: 5 }, { x: 2, y: 7 }, { x: 3, y: 10 }] },
    { series: "B", values: [{ x: 1, y: 10 }, { x: 2, y: 15 }, { x: 3, y: 17 }] }
];

// Scales
const xScale = d3.scaleLinear().domain([0, 4]).range([0, width]);
const yScale = d3.scaleLinear().domain([0, 20]).range([height, 0]);

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

