using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace ProjetClient.Views
{
    /// <summary>
    /// Logique d'interaction pour rule.xaml
    /// </summary>
    public partial class rule : Window
    {
        public rule()
        {
            InitializeComponent();
            this.txtRules.Text = "Règle du jeu\n\nChaque joueur a un certain nombre de dés à lancer. \nCes dés sont des dés à six faces normaux, excepté le 1, qu'on appelle Paco, et qui est une sorte de joker : il a toutes les autres valeurs à la fois. \nAinsi, par exemple, si on a sur cinq dés Paco-deux-trois-cinq-cinq, on comptera 2 deux (le vrai et le Paco), 2 trois(le vrai et le Paco), 1 quatre (le Paco), 3 cinq (le Paco et les deux vrais), ou 1 six (le Paco).\nLes enchères sur les faces des dés sont toujours au moins. \nSi on parie sur 5 six, il faut qu'il y ait au moins 5 dés de valeur six pour que l'enchère soit correcte.\n\nDébut\n\nOn tire d'abord aux dés qui va commencer. \nChaque joueur prend ensuite possession d'un gobelet de cinq dés, puis secoue le gobelet et le pose à l'envers, de manière que les dés aient des valeurs aléatoires et restent sous le gobelet, donc invisibles (les gobelets sont opaques).\n Chaque joueur peut alors regarder sous son gobelet et uniquement le sien. \nChaque joueur à tour de rôle, dans le sens des aiguilles d'une montre va pouvoir faire des enchères sur le nombre de dés d'une certaine valeur.\n\nDéroulement d'une manche\n\nLe premier joueur fait une enchère. Celle - ci ne doit pas porter sur le nombre de Pacos.\nLe suivant peut:\n - Surenchérir\nen pariant plus de dés: sur 7 cinq, surenchérir à 9 cinq, ou 8 deux par exemple;\nen pariant une plus grande valeur : sur 7 cinq, surenchérir à 7 six par exemple;\nen pariant le nombre de Pacos. Les Pacos étant statistiquement deux fois moins nombreux que les faces normales(Pacos inclus), on peut diviser par deux le nombre de dés pariés : sur 7 cinq, surenchérir à 4 Pacos par exemple(7 / 2 = 3, 5 donc 4 Pacos);\npour revenir d'une enchère sur les Pacos à une enchère normale, il faut multiplier par deux et ajouter un : sur 4 Pacos, surenchérir à 9 deux par exemple (4x2=8, et on ajoute 1).\n-Considérer que l'enchère est erronée(il y a moins de dés en réalité que le nombre annoncé). \nDans ce cas il dit Dudo(prononcer Doudo) et tout le monde révèle ses dés. \n - Si l'enchère était juste, le joueur qui a douté perd un dé, sinon c'est celui qui a fait l'enchère erronée qui en perd un.\n\nPoursuite du jeu\n\nLe nombre de dés pariés augmentant, il viendra fatalement un moment où l'enchère sera trop haute et où quelqu'un dira Dudo.\nQuelqu'un finira donc par perdre un dé.\nCommence alors une autre manche.\nChacun relance ses dés et celui qui vient de perdre un dé recommence les enchères.\nSi celui qui vient de perdre un dé a perdu son dernier dé, il a perdu, et c'est le joueur suivant, dans le sens des aiguilles d'une montre, qui démarre.\n\nFin du jeu\n\nLe jeu se termine quand tous les joueurs, sauf un(le vainqueur), ont perdu tous leurs dés.\n\nPalifico\n\nLe Palifico est un tour de jeu particulier, qui se produit quand un joueur vient de perdre son avant-dernier dé(il n'en a plus qu'un seul). \nLes règles sont alors modifiées pour cette manche-là uniquement: les Pacos ne sont plus des jokers et la valeur de dés enchérie par celui qui commence ne peut plus être changée.\nDe plus, celui qui commence peut parier sur les Pacos, puisqu'ils sont alors des valeurs normales.\nEn résumé, le premier joueur annonce par exemple 2 cinq, et le suivant doit dire 3 cinq, 4 cinq ou plus ; ou dire Dudo.\nSeuls les cinq seront comptés, sans les Pacos.\n";
         }

    }
}
