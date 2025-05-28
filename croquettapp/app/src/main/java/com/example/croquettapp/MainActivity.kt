package com.example.croquettapp

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Text
import androidx.compose.material3.Card
import androidx.compose.material3.HorizontalDivider
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.Alignment
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import com.example.croquettapp.ui.theme.CroquettappTheme
// variables de test
val daciaCroquettes = mapOf<String, Int>( // TOUJOURS GARDER CETTE STRUCTURE POUR LES CROQUETTES
    "type" to 1,
    "quantity" to 300,
    "frequency" to 2
)
val dacia = listOf(
    "Roger",
    "52.2",
    daciaCroquettes
)
// Ce qui sera affiché, L'application
class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        // TODO : Connection Bluetooth pour récupérer les données des chiens
        setContent {
            CroquettappTheme { // le thème de l'application, modifiable dans le dossier ui/theme
                Scaffold(modifier = Modifier) { innerPadding -> // l'affichage en barre de status / contenu / navigation
                    Column( // Une colonne pour afficher les chiens en liste
                        verticalArrangement = Arrangement.spacedBy(4.dp),
                    ) { // TODO : faire une boucle qui crée une fiche pour chaque chien
                        DogInfos(
                            modifier = Modifier.padding(innerPadding),
                            name = dacia[0].toString(),
                            weight = dacia[1].toString().toDouble(),
                            croquettes = daciaCroquettes
                            // TODO : ajouter une fonction pour modifier les croquettes et les téléverser
                        )
                    }
                }
            }
        }
    }
}

// fonction de base du projet, plus utile maintenant, à supprimer
@Composable
fun Greeting(name: String, modifier: Modifier = Modifier) {
    Text(
        text = "Hello $name!",
        modifier = modifier
    )
}

// fonction qui affiche les informations du chien sous forme de carte
@Composable
fun DogInfos(
    modifier: Modifier = Modifier, // pour modifier des paramètres d'affichage
    name: String, // nom du chien
    weight: Double, // son poids (un Double est comme un float, mais plus précis)
    croquettes: Map<String, Int>, // un dictionnaire qui contient le type de croquettes, la quantité et la fréquence
    onEditClick: () -> Unit = {} // la fonction qui s'exécute au clic sur la carte, ici celle qui modifie les croquettes
) {
    Card( // la carte qui contient les informations
        modifier = modifier
            .fillMaxWidth()
            .padding(8.dp),
        onClick = { onEditClick() }
    ) {
        Box(modifier = Modifier
                .padding(16.dp)
                .fillMaxWidth()
        ) {
            Text( // le nom du chien
                text = name,
                fontWeight = FontWeight.Bold,
                modifier = Modifier.align(Alignment.TopStart)
            )
            Text( // le poids du chien
                text = "${weight}kg",
                modifier = Modifier.align(Alignment.TopEnd)
            )
            Text( // les infos sur les croquettes
                text = "Bac: ${croquettes["type"]}, " +
                        "Qté: ${croquettes["quantity"]}g, " +
                        "Fréquence: ${croquettes["frequency"]} fois/jour",
                modifier = Modifier
                    .align(Alignment.BottomStart)
                    .padding(top = 32.dp)
            )
        }
    }
}

// une fonction pour tester directement dans le code, sans téléverser l'application
@Preview(showBackground = true)
@Composable
fun GreetingPreview() {

    CroquettappTheme {
        Scaffold(modifier = Modifier) { innerPadding ->
            Column(
                verticalArrangement = Arrangement.spacedBy(4.dp),
            ) {
                for (i in 1..3) {
                    DogInfos(
                        modifier = Modifier.padding(innerPadding),
                        name = dacia[0].toString(),
                        weight = dacia[1].toString().toDouble(),
                        croquettes = daciaCroquettes
                    )
                }
            }
        }
    }
}