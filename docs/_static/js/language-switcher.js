/**
 * Language Switcher - VMM Documentation
 * Handles language selection via flag icons in the header
 */

(function() {
  'use strict';

  // Detect current language from URL
  function detectCurrentLanguage() {
    const path = window.location.pathname;
    if (path.includes('/en-gb/')) return 'en-gb';
    if (path.includes('/pt-br/')) return 'pt-br';
    // Default to en-gb if on home page
    return 'en-gb';
  }

  // Map current page URL to target language equivalent
  function getTargetUrl(targetLang) {
    const path = window.location.pathname;
    const currentLang = detectCurrentLanguage();
    
    if (currentLang === targetLang) {
      return null; // Already on target language
    }

    // Handle root index.html or similar root-level pages
    if (!path.includes('/en-gb/') && !path.includes('/pt-br/')) {
      // We're on root - redirect to language-specific index (use relative path)
      return `./${targetLang}/index.html`;
    }

    // Replace language in path for pages inside language directories
    if (currentLang === 'en-gb') {
      return path.replace('/en-gb/', `/${targetLang}/`);
    } else {
      return path.replace('/pt-br/', `/${targetLang}/`);
    }
  }

  // Initialize language switcher
  function initLanguageSwitcher() {
    const currentLang = detectCurrentLanguage();
    const flags = document.querySelectorAll('.vmm-flag');

    flags.forEach(flag => {
      const lang = flag.getAttribute('data-lang');
      
      // Mark current language as active
      if (lang === currentLang) {
        flag.classList.add('vmm-flag-active');
      }

      // Add click handler
      flag.addEventListener('click', function(e) {
        e.preventDefault();
        const targetUrl = getTargetUrl(lang);
        if (targetUrl) {
          window.location.href = targetUrl;
        }
      });

      // Keyboard accessibility
      flag.addEventListener('keydown', function(e) {
        if (e.key === 'Enter' || e.key === ' ') {
          e.preventDefault();
          this.click();
        }
      });
    });
  }

  // Initialize when DOM is ready
  if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', initLanguageSwitcher);
  } else {
    initLanguageSwitcher();
  }
})();
