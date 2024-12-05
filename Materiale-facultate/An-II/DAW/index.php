<?php
    require_once('DB/db_connect.php'); // => $link
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Primul site PHP</title>
</head>
<body>
	<p>
		<a href="http://ro.filipcondruz.ro">
			Mergi pe ro.filipcondruz.ro
		</a>
	</p>

	<p>
		<a href="http://so.filipcondruz.ro">
			Mergi pe so.filipcondruz.ro
		</a>
	</p>

<!-- <?php
	// OpenWeather API Key
	$apiKey = "9403a78b7fdf4e1db796db3d546f425e";
	$city = "London,GB"; // Replace with your city
	$units = "metric"; // Use "imperial" for Fahrenheit, "metric" for Celsius

	// OpenWeather URL
	$url = "https://api.openweathermap.org/data/2.5/weather?q={$city}&appid={$apiKey}&units={$units}";

	// Initialize cURL
	$ch = curl_init();
	curl_setopt($ch, CURLOPT_URL, $url);
	curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);

	// Execute and get the response
	$response = curl_exec($ch);
	curl_close($ch);

	// Decode JSON data
	$data = json_decode($response, true);

	// Check if data was fetched successfully
	if ($data['cod'] === 200) {
		$weather = $data['weather'][0]['description'];
		$temperature = $data['main']['temp'];
		$humidity = $data['main']['humidity'];
		$windSpeed = $data['wind']['speed'];

		echo "<h2>Weather in {$city}</h2>";
		echo "Description: " . ucfirst($weather) . "<br>";
		echo "Temperature: " . $temperature . "°C<br>";
		echo "Humidity: " . $humidity . "%<br>";
		echo "Wind Speed: " . $windSpeed . " m/s<br>";
	} else {
		echo "Weather data not found!";
		echo "<pre>";
		print_r($data);
		echo "</pre>";
	}
?> -->

<?php
	// URL of the BBC Sport Scores and Fixtures page
	$url = 'https://www.bbc.com/sport/football/scores-fixtures';

	// Fetch the page content
	$html = file_get_contents($url);
	if ($html === FALSE) {
		die('Error fetching the page.');
	}

	// Load HTML into DOMDocument
	$dom = new DOMDocument();
	libxml_use_internal_errors(true); // Suppress HTML warnings
	$dom->loadHTML($html);
	libxml_clear_errors();

	// Create a DOMXPath instance
	$xpath = new DOMXPath($dom);

	// Find the container with class 'ssrcss-1ox7t1a-Container ea54ukl1'
	$fixturesContainer = $xpath->query("//div[contains(@class, 'ssrcss-1ox7t1a-Container') and contains(@class, 'ea54ukl1')]");

	// Check if the container was found
	if ($fixturesContainer->length > 0) {
		foreach ($fixturesContainer as $fixture) {
			// Remove all <img> tags within the fixture container
			$images = $xpath->query(".//img", $fixture);
			foreach ($images as $img) {
				$img->parentNode->removeChild($img);
			}
			
			// Output the entire HTML of the container, excluding <img> elements
			echo $dom->saveHTML($fixture);
		}
	} else {
		echo "No fixtures found for today.";
	}
?>




</body>
</html>