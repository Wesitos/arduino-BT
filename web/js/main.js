// Chart definition
var chart;
var chart2;
var new_data_0= [];
var new_data_1= [];

// Once DOM (document) is finished loading
$(document).ready(function() {

    // First chart initialization
    var options ={
	chart: {
	    renderTo: 'chart',

	    events: {
		load : function() 
		{	
		    // set up the updating of the chart each second
		    var series = this.series[0];
		    setInterval(function() 
				{

				    var date = chart.xAxis[0].getExtremes().dataMax;
				    $.getJSON('live-server-data.php','table=0&date='+date, 
					      function(data)
					      {
						  new_data_0 = data;
					      });
				    while (new_data_0.length>0)
				    {
					punto = new_data_0.pop()
					series.addPoint(punto, true, false);
				    }
				}, 1000);
		}
	    }
	},
	title: {
	    text: 'temp0'
	},
	rangeSelector: {
	    buttons:[
		{
		    type: 'day',
		    count: 1,
		    text: '1d'
		},
		{
		    type: 'week',
		    count: 1,
		    text: '1w'
		},
		{
		    type: 'all',
		    text: 'All'
		}
	    ]
	},
	series: [{}],

	xAxis : {
	    type : 'datetime'
	},

    };

    var options2 ={
	chart: {
	    renderTo: 'chart2',

	    events: {
		load : function() 
		{	
		    // set up the updating of the chart each second
		    var series = this.series[0];
		    setInterval(function() 
				{

				    var date = chart.xAxis[0].getExtremes().dataMax;
				    $.getJSON('live-server-data.php','table=1&date='+date, 
					      function(data)
					      {
						  new_data_1 = data;
					      });
				    while (new_data_1.length>0)
				    {
					punto = new_data_1.pop()
					series.addPoint(punto, true, false);
				    }
				}, 1000);
		}
	    }
	},
	title: {
	    text: 'temp1'
	},
	rangeSelector: {
	    buttons: [
		{
		    type: 'day',
		    count: 1,
		    text: '1d'
		},
		{
		    type: 'week',
		    count: 1,
		    text: '1w'
		},
		{
		    type: 'all',
		    text: 'All'
		}
	    ]
	},
	series: [{}],

	xAxis : {
	    type : 'datetime'
	},

    };

    $.getJSON('live-server-data.php','table=0&date=0', 
	      function(data)
	      {
		  options.series[0].data=data;
		  options2.series[0].data=data;
		  //chart= new Highcharts.Chart(options);
		  chart = new Highcharts.StockChart(options);
		  chart2 = new Highcharts.StockChart(options2);
	      });

});


