import javafx.animation.KeyFrame;
import javafx.animation.KeyValue;
import javafx.animation.Timeline;
import javafx.application.Application;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.ProgressBar;
import javafx.scene.control.ScrollPane;
import javafx.scene.control.TextField;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.layout.FlowPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import javafx.scene.text.Text;
import javafx.stage.Stage;
import javafx.util.Duration;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class GreedyCoinChangeModified extends Application {

    @Override
    public void start(Stage primaryStage) {
        // Layout principal
        VBox root = new VBox(10);

        // Elemente UI (partea de sus)
        VBox topBox = new VBox(10);
        topBox.setAlignment(Pos.TOP_CENTER);

        Label instructionLabel = new Label("Introduceți suma în lei (poate include zecimale):");
        instructionLabel.setStyle("-fx-font-size: 16px; -fx-font-weight: bold; -fx-padding: 10px;");

        TextField amountField = new TextField();
        amountField.setPrefWidth(100);

        Button calculateButton = new Button("Calculează");
        calculateButton.setStyle("-fx-font-size: 14px; -fx-padding: 5px 10px;");

        HBox inputBox = new HBox(10, amountField, calculateButton);
        inputBox.setAlignment(Pos.CENTER);

        Label totalCoinsLabel = new Label();
        totalCoinsLabel.setStyle("-fx-font-size: 16px; -fx-padding: 10px;");
        VBox coinStatsBox = new VBox(5);
        coinStatsBox.setAlignment(Pos.TOP_LEFT);

        ProgressBar progressBar = new ProgressBar(0);
        progressBar.setPrefWidth(400);
        progressBar.setStyle("-fx-accent: green;");
        progressBar.setVisible(false);

        topBox.getChildren().addAll(instructionLabel, inputBox, progressBar, totalCoinsLabel, coinStatsBox);

        // Rezultate (partea centrală)
        VBox resultPane = new VBox(10);
        resultPane.setAlignment(Pos.TOP_LEFT);

        ScrollPane scrollPane = new ScrollPane(resultPane);
        scrollPane.setFitToWidth(true);
        scrollPane.setPrefHeight(400);

        // Adăugare acțiune buton
        calculateButton.setOnAction(e -> {
            resultPane.getChildren().clear(); // Resetare rezultate
            totalCoinsLabel.setText("");
            coinStatsBox.getChildren().clear();

            String input = amountField.getText();
            try {
                double amountInLei = Double.parseDouble(input);
                int amountInBani = (int) Math.round(amountInLei * 100); // Conversie lei în bani
                List<Integer> coins = calculateCoins(amountInBani);
                Map<Integer, Integer> coinCount = new HashMap<>();

                progressBar.setVisible(true);
                Timeline progressTimeline = new Timeline(
                    new KeyFrame(Duration.seconds(0), new KeyValue(progressBar.progressProperty(), 0)),
                    new KeyFrame(Duration.seconds(1), new KeyValue(progressBar.progressProperty(), 0.5))
                );

                progressTimeline.play();

                progressTimeline.setOnFinished(event -> {
                    long startTime = System.currentTimeMillis();
                    int coinDisplayLimit = 100;
                    FlowPane row50Pane = new FlowPane(10, 10);
                    row50Pane.setAlignment(Pos.TOP_LEFT);
                    FlowPane row1Pane = new FlowPane(10, 10);
                    row1Pane.setAlignment(Pos.TOP_LEFT);
                    FlowPane row5Pane = new FlowPane(10, 10);
                    row5Pane.setAlignment(Pos.TOP_LEFT);
                    FlowPane row10Pane = new FlowPane(10, 10);
                    row10Pane.setAlignment(Pos.TOP_LEFT);

                    int displayed50Count = 0;

                    for (int coin : coins) {
                        if (coin == 50) {
                            if (displayed50Count < coinDisplayLimit) {
                                ImageView coinImage = createCoinImage(coin);
                                row50Pane.getChildren().add(coinImage);
                                displayed50Count++;
                            }
                        } else if (coin == 1) {
                            ImageView coinImage = createCoinImage(coin);
                            row1Pane.getChildren().add(coinImage);
                        } else if (coin == 5) {
                            ImageView coinImage = createCoinImage(coin);
                            row5Pane.getChildren().add(coinImage);
                        } else if (coin == 10) {
                            ImageView coinImage = createCoinImage(coin);
                            row10Pane.getChildren().add(coinImage);
                        }
                        coinCount.put(coin, coinCount.getOrDefault(coin, 0) + 1);
                    }

                    if (coinCount.getOrDefault(50, 0) > coinDisplayLimit) {
                        Label moreLabel = new Label("...");
                        moreLabel.setStyle("-fx-font-size: 18px; -fx-font-weight: bold;");
                        row50Pane.getChildren().add(moreLabel);
                    }

                    resultPane.getChildren().addAll(row50Pane, row1Pane, row5Pane, row10Pane);

                    long endTime = System.currentTimeMillis();
                    double totalTime = (endTime - startTime) / 1000.0; // În secunde

                    Timeline finalProgress = new Timeline(
                        new KeyFrame(Duration.seconds(totalTime), new KeyValue(progressBar.progressProperty(), 1))
                    );

                    finalProgress.setOnFinished(finishEvent -> {
                        totalCoinsLabel.setText("Total monede: " + coins.size());

                        for (Map.Entry<Integer, Integer> entry : coinCount.entrySet()) {
                            HBox coinStatEntry = new HBox(10);
                            coinStatEntry.setAlignment(Pos.CENTER_LEFT);

                            Label statLabel = new Label("Monede de " + entry.getKey() + " bani: " + entry.getValue());
                            statLabel.setStyle("-fx-font-size: 14px;");

                            ImageView coinImage = createCoinImage(entry.getKey());
                            animate2DRotation(coinImage);

                            coinStatEntry.getChildren().addAll(statLabel, coinImage);
                            coinStatsBox.getChildren().add(coinStatEntry);
                        }

                        progressBar.setVisible(false);
                    });

                    finalProgress.play();
                });
            } catch (NumberFormatException ex) {
                totalCoinsLabel.setText("Introduceți un număr valid, de exemplu 1.5 pentru 1 leu și 50 bani!");
            }
        });

        // Adăugare componente în root
        root.getChildren().addAll(topBox, scrollPane);
        root.setSpacing(10);

        // Configurare scenă și fereastră
        Scene scene = new Scene(root, 600, 600);
        primaryStage.setScene(scene);
        primaryStage.setTitle("Greedy Coin Change - Modificat");
        primaryStage.show();
    }

    private List<Integer> calculateCoins(int amount) {
        int[] coins = {1, 5, 10, 50};
        Arrays.sort(coins);
        List<Integer> result = new ArrayList<>();

        for (int i = coins.length - 1; i >= 0; i--) {
            while (amount >= coins[i]) {
                amount -= coins[i];
                result.add(coins[i]);
            }
        }

        return result;
    }

    private ImageView createCoinImage(int coinValue) {
        Image frontImage = new Image(getClass().getResourceAsStream("/images/" + coinValue + "_face.png"));
        Image backImage = new Image(getClass().getResourceAsStream("/images/" + coinValue + "_back.png"));

        ImageView coinImage = new ImageView(frontImage);
        coinImage.setFitWidth(50);
        coinImage.setFitHeight(50);
        coinImage.getProperties().put("front", frontImage);
        coinImage.getProperties().put("back", backImage);

        return coinImage;
    }

    private void animate2DRotation(ImageView coinImage) {
        Image frontImage = (Image) coinImage.getProperties().get("front");
        Image backImage = (Image) coinImage.getProperties().get("back");

        Timeline timeline = new Timeline(
            new KeyFrame(Duration.seconds(0), e -> {
                coinImage.setScaleX(1);
                coinImage.setImage(frontImage);
            }, new KeyValue(coinImage.scaleXProperty(), 1)),

            new KeyFrame(Duration.seconds(0.5), e -> {
                coinImage.setScaleX(0);
                coinImage.setImage(backImage);
            }, new KeyValue(coinImage.scaleXProperty(), 0)),

            new KeyFrame(Duration.seconds(1), e -> {
                coinImage.setScaleX(1);
                coinImage.setImage(frontImage);
            }, new KeyValue(coinImage.scaleXProperty(), 1))
        );

        timeline.setCycleCount(Timeline.INDEFINITE);
        timeline.play();
    }

    public static void main(String[] args) {
        launch(args);
    }
}