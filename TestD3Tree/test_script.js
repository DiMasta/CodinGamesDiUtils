$(document).ready(function() {
    // Load the tree data
    d3.json("test_metadata.json").then(function(treeData) {
        const width = 500;
        const height = 500;

        // Create an SVG element
        const svg = d3.select("body").append("svg")
            .attr("width", width)
            .attr("height", height);

        // Create a tree layout
        const treeLayout = d3.tree().size([width - 100, height - 100]);

        // Convert the tree data into a hierarchy
        const root = d3.hierarchy(treeData);

        // Generate the tree layout
        treeLayout(root);

        // Create links between nodes
        svg.selectAll('line')
            .data(root.links())
            .enter()
            .append('line')
            .attr('x1', function(d) { return d.source.x + 50; })
            .attr('y1', function(d) { return d.source.y + 50; })
            .attr('x2', function(d) { return d.target.x + 50; })
            .attr('y2', function(d) { return d.target.y + 50; })
            .style('stroke', 'black');

        // Create nodes
        const nodes = svg.selectAll('circle')
            .data(root.descendants())
            .enter()
            .append('circle')
            .attr('cx', function(d) { return d.x + 50; })
            .attr('cy', function(d) { return d.y + 50; })
            .attr('r', 20)
            .style('fill', function(d, i) {
                return i === 0 ? 'red' : i === 1 ? 'green' : 'blue';
            });

        // Add labels
        svg.selectAll('text')
            .data(root.descendants())
            .enter()
            .append('text')
            .attr('x', function(d) { return d.x + 50; })
            .attr('y', function(d) { return d.y + 50; })
            .attr('dy', -25)
            .attr('text-anchor', 'middle')
            .text(function(d) { return d.data.name; });
    }).catch(function(error) {
        console.error('Error loading the tree data:', error);
    });
});
