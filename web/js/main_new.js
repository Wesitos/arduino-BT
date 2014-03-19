
var generate_options = function( data_origin, id_sensor, div_id, chart_title )
{
    var opts = {};

    opts.data_origin = data_origin;
    opts.id_sensor = id_sensor;
    //Opciones comunes
    opts.series = [{}];
    opts.xAxis = { type : "datetime" };
    opts.rangeSelector ={ 
	buttons: [
	{ type: "day", count: 1, text: "1d" },
	{ type: "week", count: 1, text: "1w" },
	{ type: "all", text: "All" }
	],
	selected: 2
    };

    opts.series = [{}];
    opts.xAxis = { type: "datetime" };
    //
    
    opts.title = { text: chart_title };
    opts.chart = {};
    opts.chart.style = {
	display : "inline-block"
    }
    opts.chart.width = 800;
    // id del div donde se va a inyectar
    opts.chart.renderTo = div_id; 

    opts.chart.events = {};
    
    //funcion que se ejecutará al cargar la pagina
    opts.chart.events.load = function()
    {
	//Consigue un metodo del objeto "chart"
	var chart = this;
	var series = chart.series[0];
	
	//Esta es la función que se va a correr periodicamente
	var periodic = function()
	{
	    var date;
	    date = chart.xAxis[0].getExtremes().dataMax;
	    if (! (date > 0) )
		date = 0;


	    var update_data = function(data)
	    {
		while ( data.length > 0 )
		{
		    punto = data.pop();
		    series.addPoint(punto, true, false);
		}

	    }
	    //Consigue la data por medio de la api
	    $.getJSON(data_origin + "/"+ id_sensor + "/"+ date, 
		      "", 
		      update_data );

	}

	//Llamamos a la función cada cierto tiempo
	setInterval( periodic , 2500);
    }
    
    return opts;
}

var get_data = function (id_sensor, data_origin, callback)
{
    $.getJSON(data_origin + "/" + id_sensor + "/0",
	      "",
	      callback);
}

var generate_chart = function(options)
{
    get_data(options.id_sensor, options.data_origin, 
	     function(data)
	     {
		 options.series[0].data = data;
		 new Highcharts.StockChart(options);
	     })
}
