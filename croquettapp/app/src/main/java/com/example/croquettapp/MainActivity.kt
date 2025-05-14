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

val daciaCroquettes = mapOf<String, Int>(
    "type" to 1,
    "quantity" to 300,
    "frequency" to 2
)
val dacia = listOf(
    "Roger",
    "52.2",
    daciaCroquettes
)

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContent {
            CroquettappTheme {
                Scaffold(modifier = Modifier) { innerPadding ->
                    Column(
                        verticalArrangement = Arrangement.spacedBy(8.dp),
                    ) {
                        Text("First item in list")
                        HorizontalDivider(thickness = 2.dp)
                        Text("Second item in list")
                    }
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

@Composable
fun Greeting(name: String, modifier: Modifier = Modifier) {
    Text(
        text = "Hello $name!",
        modifier = modifier
    )
}

@Composable
fun DogInfos(
    modifier: Modifier = Modifier,
    name: String,
    weight: Double,
    croquettes: Map<String, Int>,
    onEditClick: () -> Unit = {}
) {
    Card(
        modifier = modifier
            .fillMaxWidth()
            .padding(8.dp),
        onClick = { onEditClick() }
    ) {
        Box(modifier = Modifier
                .padding(16.dp)
                .fillMaxWidth()
        ) {
            Text(
                text = name,
                fontWeight = FontWeight.Bold,
                modifier = Modifier.align(Alignment.TopStart)
            )
            Text(
                text = "${weight}kg",
                modifier = Modifier.align(Alignment.TopEnd)
            )
            Text(
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