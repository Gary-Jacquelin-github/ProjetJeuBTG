using Microsoft.Extensions.DependencyInjection;
using ProjetClient.ViewModels;
using System;
using System.Collections.Generic;
using System.Windows;

namespace ProjetClient.Services
{
    public class NavigationService
    {
        private readonly IServiceProvider serviceProvider;
        private readonly Dictionary<object, Window> modalWindows;
        private readonly Dictionary<object, Window> nonModalWindows;

        public NavigationService(IServiceProvider serviceProvider)
        {
            this.serviceProvider = serviceProvider;
            modalWindows = new Dictionary<object, Window>();
            nonModalWindows = new Dictionary<object, Window>();
        }

        private TViewModel GetViewModelInstance<TViewModel>(params object[] viewModelParameters)
            where TViewModel : BaseViewModel
        {
            return (TViewModel)ActivatorUtilities.CreateInstance(serviceProvider, typeof(TViewModel), viewModelParameters);
        }

        private TView GetViewInstance<TView>(object viewModel)
            where TView : class
        {
            Type tView = typeof(TView);
            object view = Activator.CreateInstance(tView);
            var prop = tView.GetProperty("DataContext");
            prop?.SetValue(view, viewModel);
            return (TView)view;
        }

        public TView Show<TView, TViewModel>(params object[] viewModelParameters)
            where TView : Window
            where TViewModel : BaseViewModel
        {
            var vm = GetViewModelInstance<TViewModel>(viewModelParameters);
            var view = GetViewInstance<TView>(vm);
            nonModalWindows.Add(vm, view);
            view.Closing += (s, e) =>
            {
                if (s is Window w && nonModalWindows.ContainsKey(w.DataContext))
                    nonModalWindows.Remove(w.DataContext);
            };
            view.Show();
            return view;
        }

        public void Close(Window view, bool? result = null)
        {
            if (modalWindows.ContainsKey(view.DataContext) && result != null)
                view.DialogResult = result;
            view.Close();
        }
        public bool Close<TViewModel>(TViewModel vm, bool? result = null)
            where TViewModel : BaseViewModel
        {
            if (nonModalWindows.ContainsKey(vm))
            {
                Close(nonModalWindows[vm], result);
                return true;
            }
            else if (modalWindows.ContainsKey(vm))
            {
                Close(modalWindows[vm], result);
                return true;
            }
            return false;
        }

        public bool? ShowDialog<TView, TViewModel>(params object[] viewModelParameters)
            where TView : Window
            where TViewModel : BaseViewModel
        {
            var vm = GetViewModelInstance<TViewModel>(viewModelParameters);
            var view = GetViewInstance<TView>(vm);
            modalWindows.Add(vm, view);
            var result = view.ShowDialog();
            if (modalWindows.ContainsKey(vm))
                modalWindows.Remove(vm);
            return result;
        }

    }
}
