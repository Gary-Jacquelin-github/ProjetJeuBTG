using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using ProjetClient.Services;

namespace ProjetClient.ViewModels
{
    public class MainViewModel : BaseViewModel
    {
       
        public MainViewModel(NavigationService navigation)
            : base(navigation)
        { }
    }
}
