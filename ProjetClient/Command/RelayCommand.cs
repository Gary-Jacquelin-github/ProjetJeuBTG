using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace ProjetClient.Command
{
    public class RelayCommand : ICommand
    {
        public event EventHandler CanExecuteChanged;
        private Func<bool> canExecute;
        private Action execute;

        public RelayCommand(Action execute, Func<bool> canExecute = null)
        {
            this.execute = execute ?? throw new ArgumentNullException(nameof(execute));
            this.canExecute = canExecute;
        }

        public bool CanExecute(object parameter)
        {
            return canExecute?.Invoke() ?? true;
        }

        public void Execute(object parameter)
        {
            if (CanExecute(parameter))
                execute?.Invoke();
        }
    }
}
